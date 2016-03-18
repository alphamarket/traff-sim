<?php

$ops = [ "get_info", "init", "feedback", "action", "help" ];

function _fail($code, $detail) {
	header("HTTP/1.1 $code Failure");
    die(json_encode([
    		"code" => $code,
    		"result" => "failed",
    		"detail" => $detail
    	]));
}

function _hash($str) { return sha1(md5("$$str$")); }
