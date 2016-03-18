<?php

require_once("utilities.php");

ini_set('max_execution_time', 0);

defined('SERVER_PORT') || define("SERVER_PORT", 2004);
defined('DEBUG_LEVEL') || define("DEBUG_LEVEL", "DEBUG");
defined('SERVER_ADDRESS') || define("SERVER_ADDRESS", "127.0.0.1");

if(DEBUG_LEVEL !== "DEBUG" && $_SERVER['REQUEST_METHOD'] !== "POST") _fail(403, "Invalid request!");

function send_data(&$socket, $msg, $append_eof = true) {
	$flag = 0;
	if($append_eof) $msg .= "\n";
	if(!is_string($msg)) _fail(500.101, "Invalid data type!");
	socket_send($socket, $msg, strlen($msg), $flag);
}

$data_out = $_REQUEST;

if(!count($data_out)) _fail(500.103, "Invalid data!");

if(($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) _fail(socket_last_error(), "Unable to create socket!");

if(socket_connect($socket, SERVER_ADDRESS, SERVER_PORT) === false) _fail(socket_last_error(), "Unable to connect to server!");

send_data($socket, json_encode($data_out));

$data_in = socket_read($socket, 1024 * 1024 * 10 /* 10 MB */);

socket_close($socket);

if(!strlen($data_in)) _fail(500.104, "Invalid response from server!");

die($data_in);