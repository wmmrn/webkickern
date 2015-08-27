<?php

if (function_exists('apc_clear_cache')) {
  apc_clear_cache();
  apc_clear_cache('user');
}
