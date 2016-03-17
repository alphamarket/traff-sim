<?php require_once("utilities.php"); ?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta content="IE=edge" http-equiv="X-UA-Compatible">
    <meta content="width=device-width, initial-scale=1" name="viewport">
    <title>Traffic Control Demo</title>
    <!-- Bootstrap Core CSS -->
    <link href="/statics/css/bootstrap.min.css" rel="stylesheet">
    <link href="/statics/css/font-awesome.min.css" rel="stylesheet" >
    <link href="/statics/css/scrolling-nav.min.css" rel="stylesheet">
    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
        <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->
</head>
<body style='overflow: hidden;'>


<style type="text/css">
	.section {
		border-bottom: 3px solid #e6e6e6; width: 33%;
		padding-bottom: 10px;
	}
	.full-height {
		display: none;
	}
</style>

<div class="row">
  	<div id="mynetwork" class="full-height col-md-8" style="border-right:1px solid #ddd">
	</div>
	<div id="sidebar" class="full-height col-md-4"  style="padding: 20px; padding-right: 30px; position: relative; min-height: 100px">
		<h2 class="pull-left">TrafficMeter</h2>
		<h3 class="pull-right"><small>developed by <a href="http://github.com/noise2" target="__blank"><span class='glyphicon glyphicon-new-window'></span> Dariush Hasanpoor</a></small></h3>
		<div class="clearfix"></div>
		<hr />
		<h4 class='text-muted section'>Options</h4>
		<fieldset>
		</fieldset>
		<div id="log" style='height: 50%; overflow-x: hidden; word-wrap: break-word; overflow-y: auto; display: none;'>
			<h4 class='text-muted section'>Logs</h4>
		</div>
	</div>
</div>


<script src="/statics/js/jquery.min.js" type="text/javascript"></script> 
<script src="/statics/js/bootstrap.min.js" type="text/javascript"></script> 
<script type="text/javascript" src="vis/vis.js"></script>
<link href="vis/vis.css" rel="stylesheet" type="text/css" />
   
<script type="text/javascript">
	function echo(str) {
		document.write(str);
	}
	$(document).ready(function(){<?php // [ "get_info", "init", "feedback", "action", "help" ]; ?>
		var data_help 		= function() 
			{ return {op: "help"}; };
		var data_feedback 	= function() 
			{ return { op: "feedback" }; };
		var data_get_info 	= function() 
			{ return { op: "get_info" }; };
		var data_init     	= function(_size, _cars_no, _time_step, _cluster_delay) { 
			return {
		 		op: "init",
		 		params: {
			 		size: _size,
			 		cars_no: _cars_no,
			 		time_step: _time_step,
			 		cluster_delay: _cluster_delay
		 		}
			}; 
		};
		var data_action 	= function(action, value) { 
			var value = typeof value !== 'undefined' ?  value : [];
			return { op: "action", action: action, value: value }; 
		};
		var e = [ <?php foreach ($ops as $value) echo "'", _hash($value), "', "; ?> ];
		var p = [ <?php
			foreach($ops as $value) {
				switch($value) {
					case "help":
					case "get_info":
					case "feedback": echo "[],"; break;
					case "init": echo "[-1,-1,-1,-1],"; break;
					case "action": echo "[-1,-1,-1],"; break;
					default: _fail(500.001, "invalid value!");
				}
			}
		?> ];
    	$.ajax({
			url: "net.php",
			method: "POST",
			data: data_init([4,4], 3,1,10),
			cache: false,
			success: function(data) {
				console.log("FUCK");
				$("#log").append(data);
				console.log([234, 5, 5, 5])
			}
		});

		var nodes = null;
	    var edges = null;
	    var network = null;

	    function draw() {
	      // create people.
	      // value corresponds with the age of the person
	      nodes = [
	        {id: 1,  value: 2,  label: 'Algie', x: 0, y:0 },
	        {id: 2,  value: 31, label: 'Alston'},
	        {id: 3,  value: 12, label: 'Barney'},
	        {id: 4,  value: 16, label: 'Coley' },
	        {id: 5,  value: 17, label: 'Grant' },
	        {id: 6,  value: 15, label: 'Langdon'},
	        {id: 7,  value: 6,  label: 'Lee'},
	        {id: 8,  value: 5,  label: 'Merlin'},
	        {id: 9,  value: 30, label: 'Mick'},
	        {id: 10, value: 18, label: 'Tod'},
	      ];

	      // create connections between people
	      // value corresponds with the amount of contact between two people
	      edges = [
	        {from: 2, to: 8, value: 3, title: '3 emails per week'},
	        {from: 2, to: 9, value: 5, title: '5 emails per week'},
	        {from: 2, to: 10,value: 1, title: '1 emails per week'},
	        {from: 4, to: 6, value: 8, title: '8 emails per week'},
	        {from: 5, to: 7, value: 2, title: '2 emails per week'},
	        {from: 4, to: 5, value: 1, title: '1 emails per week'},
	        {from: 9, to: 10,value: 2, title: '2 emails per week'},
	        {from: 2, to: 3, value: 6, title: '6 emails per week'},
	        {from: 3, to: 9, value: 4, title: '4 emails per week'},
	        {from: 5, to: 3, value: 1, title: '1 emails per week'},
	        {from: 2, to: 7, value: 4, title: '4 emails per week'}
	      ];

	      // Instantiate our network object.
	      var container = document.getElementById('mynetwork');
	      var z = [];
	      nodes = [];
	      edges = [];
	      var coor2id = function(x, y, x_max) { return x * x_max + y; };
	      var height = 10;
	      var width = 10;
	      for(var i = 0; i < height; i++) {
			for(var j = 0; j < width; j++) {
				e = [];
				if(i != height - 1)
					e.push({from: coor2id(i,j,height), to: coor2id(i+1,j,height), value: 1, color: "#D2E5FF"});
				if(j != width - 1)
					e.push({from: coor2id(i,j,height), to: coor2id(i,j+1,height), value: 1, color: "#D2E5FF"});
				edges = edges.concat(e);
				var val = 0;
				for(var v = 0; v < e.length; v++) val += e[v].value;
				nodes.push({id: coor2id(i,j,height), value: Math.log(v + 1), title: (v + 1), label: "["+i+","+j+"]", x: i * i * 30, y: j * j *30 });
	      	}
	      }
	      console.log(nodes);
	      var options = {
	        nodes: {
        		fixed: true,
				shape: 'dot',
				scaling:{
					min: 10,
					max: 15,
					label: {
						min:25,
						max:25
					}
				}
	        },
	        layout: {randomSeed:0}
	      };
	      var data = {
	        nodes: nodes,
	        edges: edges
	      };
	      network = new vis.Network(container, data, options);
	    }
	    $(".full-height").css({ "height": $(document).height() }).fadeIn(1000);
	    $('#log').css({ "margin-top":  $(document).height() -  $("#log").height() - 180 }).fadeIn(1000);
	    draw();
	});
</script>
</body>
</html>