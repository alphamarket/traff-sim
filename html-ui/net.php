<?php

ini_set('max_execution_time', 0);

function fail($code, $detail) {
    die(json_encode([
    		"code" => $code,
    		"result" => "failed",
    		"detail" => $detail
    	]));
}

$addr = "127.0.0.12";
$port = 2004;

if (($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) fail(socket_last_error(), "Unable to create socket!");
if(socket_connect($socket, $addr, $port) === false) fail(socket_last_error(), "Unable to connect to server!");

function send_data(&$socket, $msg, $append_eof = true) {
	$flag = 0;
	if($append_eof) $msg .= "\n";
	if(!is_string($msg)) fail(500.101, "Invalid data type!");
	socket_send($socket, $msg, strlen($msg), $flag);
}

$data_out = [
	"op" => "help",
	"params" =>  [
		"size" =>  [100, 100],
		"car_no" =>  10,
		"time_step" =>  1,
		"cluster_delay" =>  10
	]
];

send_data($socket, json_encode($data_out));

$data_in = json_decode(socket_read($socket, 1024 * 1024 * 10 /* 10 MB */));

socket_close($socket);

if(!$data_in) if(!is_string($msg)) fail(500.102, "Invalid response from server!");

echo "<pre>";
print_r($data_in);
echo "</pre>";