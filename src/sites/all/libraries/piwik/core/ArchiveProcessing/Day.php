<?php
/**
 * Piwik - Open source web analytics
 * 
 * @link http://piwik.org
 * @license http://www.gnu.org/licenses/gpl-3.0.html GPL v3 or later
 * @version $Id: Day.php 4786 2011-05-23 10:38:42Z matt $
 * 
 * @category Piwik
 * @package Piwik
 */

/**
 * Handles the archiving process for a day. 
 * The class provides generic helper methods to manipulate data from the DB, 
 * easily create Piwik_DataTable objects from running SELECT ... GROUP BY on the log_visit table.
 * 
 * All the logic of the archiving is done inside the plugins listening to the event 'ArchiveProcessing_Day.compute'
 * 
 * @package Piwik
 * @subpackage Piwik_ArchiveProcessing
 */
class Piwik_ArchiveProcessing_Day extends Piwik_ArchiveProcessing
{
	function __construct()
	{
		parent::__construct();
		$this->db = Zend_Registry::get('db');
	}

	/**
	 * Main method to process logs for a day. The only logic done here is computing the number of visits, actions, etc.
	 * All the other reports are computed inside plugins listening to the event 'ArchiveProcessing_Day.compute'.
	 * See some of the plugins for an example eg. 'Provider'
	 */
	protected function compute()
	{
		if(!$this->isThereSomeVisits())
		{
			return;
		}
		Piwik_PostEvent('ArchiveProcessing_Day.compute', $this);
	}
	
	/**
	 * Returns true if there are logs for the current archive.
	 * 
	 * If the current archive is for a specific plugin (for example, Referers), 
	 *   (for example when a Segment is defined and the Keywords report is requested)
	 * Then the function will create the Archive for the Core metrics 'VisitsSummary' which will in turn process the number of visits
	 * 
	 *  If there is no specified segment, the SQL query will always run. 
	 */
	public function isThereSomeVisits()
	{
		if(!is_null($this->isThereSomeVisits))
		{
			if($this->isThereSomeVisits && is_null($this->nb_visits)) { debug_print_backtrace(); exit; }
			return $this->isThereSomeVisits;
		}
		// Handling Custom Segment
		$segmentSql = $this->getSegment()->getSql();
		$sqlSegmentBind = $segmentSql['bind'];
		$sqlSegment = $segmentSql['sql'];
		if(!empty($sqlSegment)) $sqlSegment = ' AND '.$sqlSegment;

		// We check if there is visits for the requested date / site / segment
		//  If no specified Segment 
		//  Or if a segment is passed and we specifically process VisitsSummary
		//   Then we check the logs. This is to ensure that this query is ran only once for this day/site/segment (rather than running it for every plugin)  
		if(empty($sqlSegment) 
			|| self::getPluginBeingProcessed($this->getRequestedReport()) == 'VisitsSummary')
		{
			$query = "SELECT 	count(distinct idvisitor) as nb_uniq_visitors, 
								count(*) as nb_visits,
								sum(visit_total_actions) as nb_actions, 
								max(visit_total_actions) as max_actions, 
								sum(visit_total_time) as sum_visit_length,
								sum(case visit_total_actions when 1 then 1 else 0 end) as bounce_count,
								sum(case visit_goal_converted when 1 then 1 else 0 end) as nb_visits_converted
						FROM ".Piwik_Common::prefixTable('log_visit')." AS log_visit
						WHERE visit_last_action_time >= ?
							AND visit_last_action_time <= ?
							AND idsite = ?
							$sqlSegment
						ORDER BY NULL";
			$bind = array_merge(array($this->getStartDatetimeUTC(), $this->getEndDatetimeUTC(), $this->idsite )
								, $sqlSegmentBind);
//			echo "Querying logs...";
//			var_dump($query);var_dump($bind);
			
			$row = $this->db->fetchRow($query, $bind );
			if($row === false || $row === null || $row['nb_visits'] == 0)
			{
				$this->isThereSomeVisits = false;
				return $this->isThereSomeVisits;
			}
	
			foreach($row as $name => $value)
			{
				$this->insertNumericRecord($name, $value);
			}
			$this->setNumberOfVisits($row['nb_visits']);
			$this->setNumberOfVisitsConverted($row['nb_visits_converted']);
			$this->isThereSomeVisits = true;
			return $this->isThereSomeVisits;
		}
		
		// If a segment is specified but a plugin other than 'VisitsSummary' is being requested
		// Then we create an archive for processing VisitsSummary Core Metrics, which will in turn execute the $query above
		$archive = new Piwik_Archive_Single();
		$archive->setSite( $this->site );
		$archive->setPeriod( $this->period );
		$archive->setSegment( $this->getSegment() );
		$archive->setRequestedReport( 'VisitsSummary' );
		
		$nbVisits = $archive->getNumeric('nb_visits');
		$isThereSomeVisits = $nbVisits > 0;
		if($isThereSomeVisits)
		{
			$nbVisitsConverted = $archive->getNumeric('nb_visits_converted');
			$this->setNumberOfVisits($nbVisits);
			$this->setNumberOfVisitsConverted($nbVisitsConverted);
		}
		$this->isThereSomeVisits = $isThereSomeVisits;
		return $this->isThereSomeVisits;
	}
	
	/**
	 * Helper function that returns a DataTable containing the $select fields / value pairs.
	 * IMPORTANT: The $select must return only one row!!
	 * 
	 * Example $select = "count(distinct( config_os )) as countDistinctOs, 
	 * 						sum( config_flash ) / count(distinct(idvisit)) as percentFlash "
	 * 		   $labelCount = "test_column_name"
	 * will return a dataTable that looks like
	 * 		label  				test_column_name  	
	 * 		CountDistinctOs 	9 	
	 * 		PercentFlash 		0.5676
	 * 						
	 *
	 * @param string $select 
	 * @param string $labelCount
	 * @return Piwik_DataTable
	 */
	public function getSimpleDataTableFromSelect($select, $labelCount)
	{
		// Handling Custom Segment
		$segmentSql = $this->getSegment()->getSql();
		$sqlSegmentBind = $segmentSql['bind'];
		$sqlSegment = $segmentSql['sql'];
		if(!empty($sqlSegment)) $sqlSegment = ' AND '.$sqlSegment;
		
		$query = "SELECT $select 
			 	FROM ".Piwik_Common::prefixTable('log_visit')." AS log_visit
			 	WHERE visit_last_action_time >= ?
						AND visit_last_action_time <= ?
			 			AND idsite = ?
			 			$sqlSegment";
		$bind = array_merge(array( $this->getStartDatetimeUTC(), $this->getEndDatetimeUTC(), $this->idsite ),
							$sqlSegmentBind);
		$data = $this->db->fetchRow($query, $bind);
		
		foreach($data as $label => &$count)
		{
			$count = array($labelCount => $count);
		}
		$table = new Piwik_DataTable();
		$table->addRowsFromArrayWithIndexLabel($data);
		return $table;
	}

	public function queryActionsByDimension($label, $where = '')
	{
	    if(is_array($label))
	    {
	    	$label2 = $label;
	    	foreach($label2 as &$field) { $field = 'log_link_visit_action.'. $field; }
	        $groupBy = implode(", ", $label2);
	    	foreach($label2 as $id => &$field) { $field = "$field AS ".$label[$id]; }
	        $select = implode(", ", $label2);
	    }
	    else
	    {
	        $select = $label . " AS label ";
	        $groupBy = 'label';
	    }
	    
	    if(!empty($where)) 
	    {
	    	$where = sprintf($where, "log_link_visit_action", "log_link_visit_action");
	        $where = ' AND '.$where;
	    }
	    
		/*
		 * Handling a custom segment when processing Page reports
		 */
		$segment = $this->getSegment();
		$segmentSql = $segment->getSql($this->getSegmentsAvailableForActions(), 'log_link_visit_action');
		$sqlJoinVisitTable = $segmentSql['sql_join_visits'];
		$sqlSegmentWhere = '';
		if(!$segment->isEmpty())
		{
			$sqlSegmentWhere = ' AND '.$segmentSql['sql'];
		}

		/*
		 * Page URLs and Page names, general stats
		 */
		$query = "SELECT $select,
							count(distinct log_link_visit_action.idvisit) as `". Piwik_Archive::INDEX_NB_VISITS ."`,
							count(distinct log_link_visit_action.idvisitor) as `". Piwik_Archive::INDEX_NB_UNIQ_VISITORS ."`,
							count(*) as `". Piwik_Archive::INDEX_NB_ACTIONS ."`
					FROM ".Piwik_Common::prefixTable('log_link_visit_action')." as log_link_visit_action
							$sqlJoinVisitTable
					WHERE server_time >= ?
						AND server_time <= ?
						AND log_link_visit_action.idsite = ?
						$where
				 		$sqlSegmentWhere
					GROUP BY $groupBy";
				 		
		$bind = array_merge( array( $this->getStartDatetimeUTC(), 
                                    $this->getEndDatetimeUTC(), 
                                    $this->idsite ),
                             $segmentSql['bind']);
		$query = $this->db->query($query, $bind );
	    return $query;
	}
	
	/**
	 * Query visits by dimension
	 *
	 * @param string $label mixed Can be a string, eg. "referer_name", will be aliased as 'label' in the returned rows
	 * 				Can also be an array of strings, when the dimension spans multiple fields, eg. array("referer_name", "referer_keyword") 
	 * @param string $where Additional condition for WHERE clause
	 */
	public function queryVisitsByDimension($label, $where = '')
	{
	    if(is_array($label))
	    {
	        $groupBy = implode(", ", $label);
	    	foreach($label as &$field)
	    	{
	    		$field = 'log_visit.'.$field.' AS '.$field;
	    	}
	        $select = implode(", ", $label);
	    }
	    else
	    {
	        $select = $label . " AS label ";
	        $groupBy = 'label';
	    }
	    
	    if(!empty($where)) 
	    {
	    	$where = sprintf($where, "log_visit", "log_visit");
	        $where = ' AND '.$where;
	    }
	    
	    $segmentSql = $this->segment->getSql();
	    $segment = '';
	    if(!empty($segmentSql['sql']))
	    {
	        $segment = ' AND '.$segmentSql['sql'];
	    } 
	    
		$query = "SELECT 	$select,
							count(distinct idvisitor) as `". Piwik_Archive::INDEX_NB_UNIQ_VISITORS ."`, 
							count(*) as `". Piwik_Archive::INDEX_NB_VISITS ."`,
							sum(visit_total_actions) as `". Piwik_Archive::INDEX_NB_ACTIONS ."`, 
							max(visit_total_actions) as `". Piwik_Archive::INDEX_MAX_ACTIONS ."`, 
							sum(visit_total_time) as `". Piwik_Archive::INDEX_SUM_VISIT_LENGTH ."`,
							sum(case visit_total_actions when 1 then 1 else 0 end) as `". Piwik_Archive::INDEX_BOUNCE_COUNT ."`,
							sum(case visit_goal_converted when 1 then 1 else 0 end) as `". Piwik_Archive::INDEX_NB_VISITS_CONVERTED ."`
				FROM ".Piwik_Common::prefixTable('log_visit')." AS log_visit
				WHERE visit_last_action_time >= ?
						AND visit_last_action_time <= ?
						AND idsite = ?
						$where
						$segment
				GROUP BY $groupBy
				ORDER BY NULL";
		$bind = array_merge( array( $this->getStartDatetimeUTC(), 
                                    $this->getEndDatetimeUTC(), 
                                    $this->idsite ),
                             $segmentSql['bind']);
		$query = $this->db->query($query, $bind );
	    return $query;
	}

	public function getSegmentsAvailableForActions()
	{
	    $allowedSegments = array(
	    		'idvisitor',
                'custom_var_k1',
                'custom_var_v1',
                'custom_var_k2',
                'custom_var_v2',
                'custom_var_k3',
                'custom_var_v3',
                'custom_var_k4',
                'custom_var_v4',
                'custom_var_k5',
                'custom_var_v5',
	    );
	    return $allowedSegments;
	}
	
	protected function getSegmentsAvailableForConversions()
	{
	    $allowedSegments = array(
	    		'idvisitor',
                'referer_type',
                'referer_name',
                'referer_keyword',
                'visitor_returning',
	    		'visitor_days_since_first',
	    		'visitor_days_since_order',
	    		'visitor_count_visits',
	    		'visit_goal_buyer',
                'location_country',
                'location_continent',
                'revenue',
                'custom_var_k1',
                'custom_var_v1',
                'custom_var_k2',
                'custom_var_v2',
                'custom_var_k3',
                'custom_var_v3',
                'custom_var_k4',
                'custom_var_v4',
                'custom_var_k5',
                'custom_var_v5',
	    );
	    return $allowedSegments;
	}
	
	/**
	 * @see queryVisitsByDimension() Similar to this function, 
	 * but queries metrics for the requested dimensions, 
	 * for each Goal conversion 
	 */
	public function queryConversionsByDimension($label, $where = '')
	{
		if(empty($label))
		{
			$select = "";
			$groupBy = "";
		}
	    elseif(is_array($label))
	    {
	        $groupBy = implode(", ", $label);
	    	foreach($label as &$field)
	    	{
	    		$field = 'log_conversion.'.$field.' AS '.$field ;
	    	}
	        $select =  implode(", ", $label) . ", ";
	    }
	    else
	    {
	        $select = $label . " AS label, ";
	        $groupBy = 'label';
	    }
	    if(!empty($where)) 
	    {
	    	$where = sprintf($where, "log_conversion", "log_conversion");
	        $where = ' AND '.$where;
	    }
	    
	    $segment = $this->segment;
		$segmentSql = $segment->getSql($this->getSegmentsAvailableForConversions(), 'log_conversion');
		$sqlJoinVisitTable = $segmentSql['sql_join_visits'];
	    $segment = '';
	    if(!empty($segmentSql['sql']))
	    {
	        $segment = ' AND '.$segmentSql['sql'];
	    }
		
    	$select .= 	self::getSqlRevenue('SUM(revenue_subtotal)')." as `". Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SUBTOTAL ."`,".
		    		self::getSqlRevenue('SUM(revenue_tax)')." as `". Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_TAX ."`,".
		    		self::getSqlRevenue('SUM(revenue_shipping)')." as `". Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SHIPPING ."`,".
		    		self::getSqlRevenue('SUM(revenue_discount)')." as `". Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_DISCOUNT ."`,".
		    		"SUM(items) as `". Piwik_Archive::INDEX_GOAL_ECOMMERCE_ITEMS ."`, ";
		    		
	    $groupBy = !empty($groupBy) ? ", $groupBy" : '';
		$query = "SELECT 
						$select
						idgoal,
						count(*) as `". Piwik_Archive::INDEX_GOAL_NB_CONVERSIONS ."`,
						".self::getSqlRevenue('SUM(revenue)')." as `". Piwik_Archive::INDEX_GOAL_REVENUE ."`,
						count(distinct idvisit) as `". Piwik_Archive::INDEX_GOAL_NB_VISITS_CONVERTED."`
			 	FROM ".Piwik_Common::prefixTable('log_conversion')." AS log_conversion
			 		$sqlJoinVisitTable
			 	WHERE server_time >= ?
						AND server_time <= ?
			 			AND log_conversion.idsite = ?
			 			$where
						$segment
			 	GROUP BY idgoal $groupBy
				ORDER BY NULL";
		$bind = array_merge( array( $this->getStartDatetimeUTC(), 
                                    $this->getEndDatetimeUTC(), 
                                    $this->idsite ),
                             $segmentSql['bind']);
		$query = $this->db->query($query, $bind);
		return $query;
	}
	
	public function queryEcommerceItems($field)
	{
		$query = "SELECT 
						name as label,
						".self::getSqlRevenue('SUM(quantity * price)')." as `". Piwik_Archive::INDEX_ECOMMERCE_ITEM_REVENUE ."`,
						".self::getSqlRevenue('SUM(quantity)')." as `". Piwik_Archive::INDEX_ECOMMERCE_ITEM_QUANTITY ."`,
						".self::getSqlRevenue('SUM(price)')." as `". Piwik_Archive::INDEX_ECOMMERCE_ITEM_PRICE ."`,
						count(distinct idorder) as `". Piwik_Archive::INDEX_ECOMMERCE_ORDERS."`,
						count(idvisit) as `". Piwik_Archive::INDEX_NB_VISITS."`,
						case idorder when '0' then ".Piwik_Tracker_GoalManager::IDGOAL_CART." else ".Piwik_Tracker_GoalManager::IDGOAL_ORDER." end as ecommerceType
			 	FROM ".Piwik_Common::prefixTable('log_conversion_item')."
			 		LEFT JOIN ".Piwik_Common::prefixTable('log_action')." 
			 		ON $field = idaction
			 	WHERE server_time >= ?
						AND server_time <= ?
			 			AND idsite = ?
			 			AND deleted = 0
			 	GROUP BY ecommerceType, $field
				ORDER BY NULL";
						
		$bind = array( $this->getStartDatetimeUTC(), 
                       $this->getEndDatetimeUTC(), 
                       $this->idsite
        );
		$query = $this->db->query($query, $bind);
		return $query;
	}
	
	static public function getSqlRevenue($field)
	{
		return "ROUND(".$field.",".Piwik_Tracker_GoalManager::REVENUE_PRECISION.")";
	}
	
	public function getDataTableFromArray( $array )
	{
		$table = new Piwik_DataTable();
		$table->addRowsFromArrayWithIndexLabel($array);
		return $table;
	}
	
	/**
	 * Output:
	 * 		array(
	 * 			LABEL => array(
	 * 						Piwik_Archive::INDEX_NB_UNIQ_VISITORS 	=> 0, 
	 *						Piwik_Archive::INDEX_NB_VISITS 			=> 0
	 *					),
	 *			LABEL2 => array(
	 *					[...]
	 *					)
	 * 		)
 	 *
	 * Helper function that returns an array with common statistics for a given database field distinct values.
	 * 
	 * The statistics returned are:
	 *  - number of unique visitors
	 *  - number of visits
	 *  - number of actions
	 *  - maximum number of action for a visit
	 *  - sum of the visits' length in sec
	 *  - count of bouncing visits (visits with one page view)
	 * 
	 * For example if $label = 'config_os' it will return the statistics for every distinct Operating systems
	 * The returned array will have a row per distinct operating systems, 
	 * and a column per stat (nb of visits, max  actions, etc)
	 * 
	 * 'label'	Piwik_Archive::INDEX_NB_UNIQ_VISITORS	Piwik_Archive::INDEX_NB_VISITS	etc.	
	 * Linux	27	66	...
	 * Windows XP	12	...	
	 * Mac OS	15	36	...
	 * 
	 * @param string $label Table log_visit field name to be use to compute common stats
	 * @return array
	 */
	public function getArrayInterestForLabel($label)
	{
	    $query = $this->queryVisitsByDimension($label);
		$interest = array();
		while($row = $query->fetch())
		{
			if(!isset($interest[$row['label']])) $interest[$row['label']]= $this->getNewInterestRow();
			$this->updateInterestStats( $row, $interest[$row['label']]);
		}
		return $interest;
	}
	
	/**
	 * Generates a dataTable given a multidimensional PHP array that associates LABELS to Piwik_DataTableRows
	 * This is used for the "Actions" DataTable, where a line is the aggregate of all the subtables
	 * Example: the category /blog has 3 visits because it has /blog/index (2 visits) + /blog/about (1 visit) 
	 *
	 * @param array $table
	 * @return Piwik_DataTable
	 */
	static public function generateDataTable( $table )
	{
		$dataTableToReturn = new Piwik_DataTable();
		foreach($table as $label => $maybeDatatableRow)
		{
			// case the aInfo is a subtable-like array
			// it means that we have to go recursively and process it
			// then we build the row that is an aggregate of all the children
			// and we associate this row to the subtable
			if( !($maybeDatatableRow instanceof Piwik_DataTable_Row) )
			{
				$subTable = self::generateDataTable($maybeDatatableRow);
				$row = new Piwik_DataTable_Row_DataTableSummary( $subTable );
				$row->setColumns( array('label' => $label) + $row->getColumns());
				$row->addSubtable($subTable);
			}
			// if aInfo is a simple Row we build it
			else
			{
				$row = $maybeDatatableRow;
			}
			
			$dataTableToReturn->addRow($row);
		}
		return $dataTableToReturn;
	}
	
	/**
	 * Helper function that returns the serialized DataTable of the given PHP array.
	 * The array must have the format of Piwik_DataTable::addRowsFromArrayWithIndexLabel()
	 * Example: 	array (
	 * 	 				LABEL => array(col1 => X, col2 => Y),
	 * 	 				LABEL2 => array(col1 => X, col2 => Y),
	 * 				)
	 * 
	 * @param array $array at the given format
	 * @return array Array with one element: the serialized data table string
	 */
	public function getDataTableSerialized( $array )
	{
		$table = new Piwik_DataTable();
		$table->addRowsFromArrayWithIndexLabel($array );
		$toReturn = $table->getSerialized();
		return $toReturn;
	}
	
	
	/**
	 * Helper function that returns the multiple serialized DataTable of the given PHP array.
	 * The DataTable here associates a subtable to every row of the level 0 array.
	 * This is used for example for search engines. 
	 * Every search engine (level 0) has a subtable containing the keywords.
	 * 
	 * The $arrayLevel0 must have the format 
	 * Example: 	array (
	 * 					// Yahoo.com => array( kwd1 => stats, kwd2 => stats )
	 * 	 				LABEL => array(col1 => X, col2 => Y),
	 * 	 				LABEL2 => array(col1 => X, col2 => Y),
	 * 				)
	 * 
	 * The $subArrayLevel1ByKey must have the format
	 * Example: 	array(
	 * 					// Yahoo.com => array( stats )
	 * 					LABEL => #Piwik_DataTable_ForLABEL,
	 * 					LABEL2 => #Piwik_DataTable_ForLABEL2,
	 * 				)
	 * 
	 * 
	 * @param array $arrayLevel0
	 * @param array $subArrayLevel1ByKey Array of Piwik_DataTable
	 * @return array Array with N elements: the strings of the datatable serialized 
	 */
	public function getDataTableWithSubtablesFromArraysIndexedByLabel( $arrayLevel0, $subArrayLevel1ByKey )
	{
		$parentTableLevel0 = new Piwik_DataTable();
		
		$tablesByLabel = array();
		foreach($arrayLevel0 as $label => $aAllRowsForThisLabel)
		{
			$table = new Piwik_DataTable();
			$table->addRowsFromArrayWithIndexLabel($aAllRowsForThisLabel);
			$tablesByLabel[$label] = $table;
		}
		$parentTableLevel0->addRowsFromArrayWithIndexLabel($subArrayLevel1ByKey, $tablesByLabel);

		return $parentTableLevel0;
	}
	
	/**
	 * Returns an empty row containing default values for the common stat
	 *
	 * @return array
	 */
	public function getNewInterestRow($onlyMetricsAvailableInActionsTable = false)
	{
		if($onlyMetricsAvailableInActionsTable)
		{
			return array(	Piwik_Archive::INDEX_NB_UNIQ_VISITORS 	=> 0, 
					Piwik_Archive::INDEX_NB_VISITS 			=> 0, 
					Piwik_Archive::INDEX_NB_ACTIONS 		=> 0 );
		}
		
		return array(	Piwik_Archive::INDEX_NB_UNIQ_VISITORS 	=> 0, 
						Piwik_Archive::INDEX_NB_VISITS 			=> 0, 
						Piwik_Archive::INDEX_NB_ACTIONS 		=> 0, 
						Piwik_Archive::INDEX_MAX_ACTIONS 		=> 0, 
						Piwik_Archive::INDEX_SUM_VISIT_LENGTH 	=> 0, 
						Piwik_Archive::INDEX_BOUNCE_COUNT 		=> 0,
						Piwik_Archive::INDEX_NB_VISITS_CONVERTED=> 0,
						);
	}
	
	
	/**
	 * Returns a Piwik_DataTable_Row containing default values for common stat, 
	 * plus a column 'label' with the value $label
	 *
	 * @param string $label
	 * @return Piwik_DataTable_Row
	 */
	public function getNewInterestRowLabeled( $label )
	{
		return new Piwik_DataTable_Row(
				array( 
					Piwik_DataTable_Row::COLUMNS => 		array(	'label' => $label) 
															+ $this->getNewInterestRow()
					)
				); 
	}
	
	/**
	 * Adds the given row $newRowToAdd to the existing  $oldRowToUpdate passed by reference
	 *
	 * The rows are php arrays Name => value
	 * 
	 * @param array $newRowToAdd
	 * @param array $oldRowToUpdate
	 */
	public function updateInterestStats( $newRowToAdd, &$oldRowToUpdate, $onlyMetricsAvailableInActionsTable = false)
	{
		// Pre 1.2 format: string indexed rows are returned from the DB
		// Left here for Backward compatibility with plugins doing custom SQL queries using these metrics as string
		if(!isset($newRowToAdd[Piwik_Archive::INDEX_NB_VISITS]))
		{
    		$oldRowToUpdate[Piwik_Archive::INDEX_NB_UNIQ_VISITORS]		+= $newRowToAdd['nb_uniq_visitors'];
    		$oldRowToUpdate[Piwik_Archive::INDEX_NB_VISITS] 			+= $newRowToAdd['nb_visits'];
    		$oldRowToUpdate[Piwik_Archive::INDEX_NB_ACTIONS] 			+= $newRowToAdd['nb_actions'];
    		if($onlyMetricsAvailableInActionsTable)
    		{
    			return;
    		}
    		$oldRowToUpdate[Piwik_Archive::INDEX_MAX_ACTIONS] 		 	= (float)max($newRowToAdd['max_actions'], $oldRowToUpdate[Piwik_Archive::INDEX_MAX_ACTIONS]);
    		$oldRowToUpdate[Piwik_Archive::INDEX_SUM_VISIT_LENGTH]		+= $newRowToAdd['sum_visit_length'];
    		$oldRowToUpdate[Piwik_Archive::INDEX_BOUNCE_COUNT] 			+= $newRowToAdd['bounce_count'];
    		$oldRowToUpdate[Piwik_Archive::INDEX_NB_VISITS_CONVERTED] 	+= $newRowToAdd['nb_visits_converted'];
    		return;
		}
		$oldRowToUpdate[Piwik_Archive::INDEX_NB_UNIQ_VISITORS]		+= $newRowToAdd[Piwik_Archive::INDEX_NB_UNIQ_VISITORS];
		$oldRowToUpdate[Piwik_Archive::INDEX_NB_VISITS] 			+= $newRowToAdd[Piwik_Archive::INDEX_NB_VISITS];
		$oldRowToUpdate[Piwik_Archive::INDEX_NB_ACTIONS] 			+= $newRowToAdd[Piwik_Archive::INDEX_NB_ACTIONS];
    	if($onlyMetricsAvailableInActionsTable)
    	{
    		return;
    	}
		$oldRowToUpdate[Piwik_Archive::INDEX_MAX_ACTIONS] 		 	= (float)max($newRowToAdd[Piwik_Archive::INDEX_MAX_ACTIONS], $oldRowToUpdate[Piwik_Archive::INDEX_MAX_ACTIONS]);
		$oldRowToUpdate[Piwik_Archive::INDEX_SUM_VISIT_LENGTH]		+= $newRowToAdd[Piwik_Archive::INDEX_SUM_VISIT_LENGTH];
		$oldRowToUpdate[Piwik_Archive::INDEX_BOUNCE_COUNT] 			+= $newRowToAdd[Piwik_Archive::INDEX_BOUNCE_COUNT];
		$oldRowToUpdate[Piwik_Archive::INDEX_NB_VISITS_CONVERTED] 	+= $newRowToAdd[Piwik_Archive::INDEX_NB_VISITS_CONVERTED];
	} 
	
	
	/**
	 * Given an array of stats, it will process the sum of goal conversions 
	 * and sum of revenue and add it in the stats array in two new fields.
	 * 
	 * @param array $interestByLabel Passed by reference, it will be modified as follows:
	 * Input: 
	 * 		array( 
	 * 			LABEL  => array( Piwik_Archive::INDEX_NB_VISITS => X, 
	 * 							 Piwik_Archive::INDEX_GOALS => array(
	 * 								idgoal1 => array( [...] ), 
	 * 								idgoal2 => array( [...] ),
	 * 							),
	 * 							[...] ),
	 * 			LABEL2 => array( Piwik_Archive::INDEX_NB_VISITS => Y, [...] )
	 * 			);
	 * 
	 * 
	 * Output:
	 * 		array(
	 * 			LABEL  => array( Piwik_Archive::INDEX_NB_VISITS => X, 
	 * 							 Piwik_Archive::INDEX_NB_CONVERSIONS => Y, // sum of all conversions
	 * 							 Piwik_Archive::INDEX_REVENUE => Z, // sum of all revenue
	 * 							 Piwik_Archive::INDEX_GOALS => array(
	 * 								idgoal1 => array( [...] ), 
	 * 								idgoal2 => array( [...] ),
	 * 							),
	 * 							[...] ),
	 * 			LABEL2 => array( Piwik_Archive::INDEX_NB_VISITS => Y, [...] )
	 * 			);
	 * 		)
	 *
	 * @param array $interestByLabel Passed by reference, will be modified
	 */
	function enrichConversionsByLabelArray(&$interestByLabel)
	{
		foreach($interestByLabel as $label => &$values)
		{
			if(isset($values[Piwik_Archive::INDEX_GOALS]))
			{
				// When per goal metrics are processed, general 'visits converted' is not meaningful because
				// it could differ from the sum of each goal conversions
				unset($values[Piwik_Archive::INDEX_NB_VISITS_CONVERTED]);
				$revenue = $conversions = 0;
				foreach($values[Piwik_Archive::INDEX_GOALS] as $idgoal => $goalValues)
				{
					// Do not sum Cart revenue since it is a lost revenue
					if($idgoal >= Piwik_Tracker_GoalManager::IDGOAL_ORDER)
					{
						$revenue += $goalValues[Piwik_Archive::INDEX_GOAL_REVENUE];
						$conversions += $goalValues[Piwik_Archive::INDEX_GOAL_NB_CONVERSIONS];
					}
				}
				$values[Piwik_Archive::INDEX_NB_CONVERSIONS] = $conversions;
				
				// 25.00 recorded as 25 
				if(round($revenue) == $revenue)
				{
					$revenue = round($revenue);
				}
				$values[Piwik_Archive::INDEX_REVENUE] = $revenue;
			}
		}
	}

	/**
	 * @param array $interestByLabelAndSubLabel Passed by reference, will be modified
	 */
	function enrichConversionsByLabelArrayHasTwoLevels(&$interestByLabelAndSubLabel)
	{
		foreach($interestByLabelAndSubLabel as $mainLabel => &$interestBySubLabel)
		{
			$this->enrichConversionsByLabelArray($interestBySubLabel);
		}
	}

	function updateGoalStats($newRowToAdd, &$oldRowToUpdate)
	{
		$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_NB_CONVERSIONS]	+= $newRowToAdd[Piwik_Archive::INDEX_GOAL_NB_CONVERSIONS];
		$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_NB_VISITS_CONVERTED]	+= $newRowToAdd[Piwik_Archive::INDEX_GOAL_NB_VISITS_CONVERTED];
		$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_REVENUE] 			+= $newRowToAdd[Piwik_Archive::INDEX_GOAL_REVENUE];
		
		// Cart & Order
		if(isset($oldRowToUpdate[Piwik_Archive::INDEX_GOAL_ECOMMERCE_ITEMS]))
		{
			$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_ECOMMERCE_ITEMS] += $newRowToAdd[Piwik_Archive::INDEX_GOAL_ECOMMERCE_ITEMS];
			
			// Order only
			if(isset($oldRowToUpdate[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SUBTOTAL]))
			{
				$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SUBTOTAL] += $newRowToAdd[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SUBTOTAL];
				$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_TAX] += $newRowToAdd[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_TAX];
				$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SHIPPING] += $newRowToAdd[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SHIPPING];
				$oldRowToUpdate[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_DISCOUNT] += $newRowToAdd[Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_DISCOUNT];
			}
		}
	}
	
	function getNewGoalRow($idGoal)
	{
		if($idGoal > Piwik_Tracker_GoalManager::IDGOAL_ORDER)
		{
			return array(	Piwik_Archive::INDEX_GOAL_NB_CONVERSIONS 	=> 0, 
							Piwik_Archive::INDEX_GOAL_NB_VISITS_CONVERTED => 0, 
							Piwik_Archive::INDEX_GOAL_REVENUE 			=> 0, 
						);
		}
		if($idGoal == Piwik_Tracker_GoalManager::IDGOAL_ORDER)
		{
			return array(	Piwik_Archive::INDEX_GOAL_NB_CONVERSIONS 	=> 0, 
							Piwik_Archive::INDEX_GOAL_NB_VISITS_CONVERTED => 0, 
							Piwik_Archive::INDEX_GOAL_REVENUE 			=> 0, 
							Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SUBTOTAL => 0,
							Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_TAX => 0,
							Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_SHIPPING => 0,
							Piwik_Archive::INDEX_GOAL_ECOMMERCE_REVENUE_DISCOUNT => 0,
							Piwik_Archive::INDEX_GOAL_ECOMMERCE_ITEMS => 0,
			);
		}
		// $row['idgoal'] == Piwik_Tracker_GoalManager::IDGOAL_CART
		return array(	Piwik_Archive::INDEX_GOAL_NB_CONVERSIONS 	=> 0, 
						Piwik_Archive::INDEX_GOAL_NB_VISITS_CONVERTED => 0, 
						Piwik_Archive::INDEX_GOAL_REVENUE 			=> 0, 
						Piwik_Archive::INDEX_GOAL_ECOMMERCE_ITEMS => 0,
		);
	}
}
