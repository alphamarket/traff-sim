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
	#log {
		height: 80%;
		display: none;
	}
	#log-content {
		height: 100%;
		overflow-x: hidden; 
 		word-wrap: break-word; 
 		overflow-y: auto;
 		font-size: 14px;
 		font-weight: 500;
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
		<div class="form-group col-md-6">
		  <label for="grid-height">Height</label>
		  	<input class='col-md-4 form-control' type="number" step='1' min='0' max='100' id="grid-height" value="0"></input>
	  	</div>
		<div class="form-group col-md-6">
		  <label for="grid-width">Width</label>
		  	<input class='col-md-4 form-control' type="number" step='1' min='0' max='100' id="grid-width" value="0"></input>
		</div>
		<div class="form-group col-md-6">
		  <label for="grid-width">Cars#</label>
		  	<input class='col-md-4 form-control' type="number" min='0' step='10' id="grid-car-no" value="0" disabled></input>
		</div>
		<div class="form-group col-md-6">
		  <label for="grid-width">Add Cars</label>
		  	<input class='col-md-4 form-control' type="number" min='0' step='10' id="grid-add-cars" value="0"></input>
		</div>
		<div class="form-group col-md-6">
		  <label for="grid-width">Time Step (s)</label>
		  	<input class='col-md-4 form-control' type="number" min='0' step='0.1' id="grid-time-step" value="0"></input>
		</div>
		<div class="form-group col-md-6">
		  <label for="grid-width">Cluster Delay (ms)</label>
		  	<input class='col-md-4 form-control' type="number" min='0' step='0.1' id="grid-cluster-delay" value="0"></input>
		</div>
		<div class="form-group col-md-6">
		  <label for="grid-state">State</label>
		  <div id='grid-state'>
				<label class="radio-inline">
			  		<input type="radio" name="grid-state" id='grid-state-START' value='START'><span class='text-success'>START</span>
				</label>
				<label class="radio-inline">
			  		<input type="radio" name="grid-state" id='grid-state-PAUSE' value='PAUSE'><span class='text-info'>PAUSE</span>
				</label>
				<label class="radio-inline">
			  		<input type="radio" name="grid-state" id='grid-state-STOP' value='STOP' onclick="return confirm('Are you sure?');"><span class='text-danger'>STOP</span>
				</label>
	    	</div>
		</div>
		<div class="form-group col-md-6" style='margin-top: 0 -50px'>
			<button class='btn btn-success pull-right' id='update-init'><span class='fa fa-upload'></span> Update</button>
		</div>
		<div class="clearfix"></div>
		</fieldset>
		<div id="log" class="pull-left col-md-12" style='margin-top: -200px'>
			<h4 class='text-muted section'>Logs</h4>
			<ul class='list-group' id="log-content"></ul>
		</div>
	</div>
</div>


<script src="/statics/js/jquery.min.js" type="text/javascript"></script> 
<script src="/statics/js/bootstrap.min.js" type="text/javascript"></script> 
<script type="text/javascript" src="vis/vis.js"></script>
<link href="vis/vis.css" rel="stylesheet" type="text/css" />
   
<script type="text/javascript">
	function log(data, type) {
		$("#log-content")
			.prepend('<log class="list-group-item list-group-item-'+type+'"><i class="fa fa-angle-double-right"></i> '+data+'</li>');
	};
	function data_help() 
		{ return {op: "help"}; };
	function data_feedback() 
		{ return { op: "feedback" }; };
	function data_get_info() 
		{ return { op: "get_info" }; };
	function data_init(_size, _cars_no, _time_step, _cluster_delay) { 
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
	function data_setting(key, value) { 
		var key = typeof key !== 'undefined' ?  key : [];
		var value = typeof value !== 'undefined' ?  value : [];
		if(key.length !== value.length) throw "invalid input!"; 
		return { op: "setting", set: { key: key, value: value } }; 
	};
	function make_request(data, succ_callback) {
    	$.ajax({
			url: "net.php",
			method: "POST",
			data: data,
			cache: false,
			dataType: 'json',
			success: succ_callback,
			complete: function() { $('#log-content .trans-log').parent().remove(); },
			beforeSend: function() { log("<span class='trans-log'>Transmitting with server.....</span>"); }
		}).fail(function( xhr, textStatus ) {
			log('Error '+xhr.status+' while trying to connect with server!', 'danger');
		}).always(function() { $('#log-spin').fadeIn(); });
	};
	function is_failed_data(data, do_log) { 
		var do_log = typeof do_log !== 'undefined' ?  do_log : false;
		if(data.result === undefined || data.result !== "failed") return false;
		if(do_log) log("<b>Error " + data.code + ": " + data.detail + "</b>", "danger"); 
		return true;
	};
	function draw(grid) {
		if(grid.grid_size === undefined || grid.grid_size.length !== 2) throw "Invalid grid info!";
		// Instantiate our network object.
		var container = document.getElementById('mynetwork');
		var z = [];
		nodes = [];
		edges = [];
		var coor2id = function(x, y, x_max) { return x * x_max + y; };
		var height = grid.grid_size[0];
		var width = grid.grid_size[1];
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
		return new vis.Network(container, data, options);
	};
	function update_info(do_draw) {
		var do_draw = typeof do_draw !== 'undefined' ?  do_draw : false;
		make_request(data_get_info(), function(info) {
			if(!is_failed_data(info, true)) {
				$("#grid-height").val(info.grid_size[0]).data('oldval', info.grid_size[0]);
				$("#grid-width").val(info.grid_size[1]).data('oldval', info.grid_size[1]);
				$("#grid-car-no").val(info.car_count).data('oldval', info.car_count);
				$("#grid-time-step").val(Math.round(info.time_step*100)/100).data('oldval', info.time_step);
				$("#grid-cluster-delay").val(Math.round(info.cluster_delay*100)/100).data('oldval', info.cluster_delay);
				$("#grid-state-"+info.flow).prop("checked", true);
				if(do_draw) network = draw(info);
			}
		});
	};
	$(document).ready(function(){
		$("#update-init").click(function(){
			if($("#grid-height").val() == $("#grid-height").data('oldval') && $("#grid-width").val() == $("#grid-width").data('oldval')) {
				// the grid size remained unchanged, only update some settings
				make_request(
					data_setting(
						["flow", "time_step", "cluster_delay", "add_cars"],
						[$("input[name=grid-state]:checked").val(), $("#grid-time-step").val(), $("#grid-cluster-delay").val(), $("#grid-add-cars").val()]), 
					function(data) {
						if(!is_failed_data(data, true)) {
							$("#grid-time-step").fadeOut(500).fadeIn(1000);
							$("#grid-cluster-delay").fadeOut(500).fadeIn(1000);
							$("#grid-add-cars").fadeOut(500).val(0).fadeIn(1000);
							$("#grid-state").fadeOut(500).fadeIn(1000);
							log("<b>Settings updated successfully!</b>", 'success');
							update_info();
						}
					});

			} else {
				// since the grid's size changed, we need to re-init the grid
				make_request(
					data_init(
						[$("#grid-height").val(), $("#grid-width").val()], 
						$("#grid-add-cars").val(), 
						$("#grid-time-step").val(),
						$("#grid-cluster-delay").val()), 
					function(data) {
						if(!is_failed_data(data, true)) {
							log("<b>Settings updated successfully!</b>", 'success');
							setTimeout(function() {
								make_request(
									data_setting(
										["flow"], 
										[$("input[name=grid-state]:checked").val()]), 
									function(data) {
										if(!is_failed_data(data, true))
											window.location.reload(true);
									});
							}, 200);
						}
					});
			}
		});
	    $(".full-height").css({ "height": $(document).height() }).fadeIn(1000);
	    $('#log').css({ "margin-top":  $(document).height() -  $("#log").height() - 180 }).fadeIn(100, function() { $('#log-spin').hide(); });
		$('.only-number-allowed').keypress(function(e) { return e.charCode >= 48 && e.charCode <= 57; });

		var network = [];
		update_info(true);
	});
</script>
</body>
</html>