<?php require_once("utilities.php"); ?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta content="IE=edge" http-equiv="X-UA-Compatible">
    <meta content="width=device-width, initial-scale=1" name="viewport">
    <title>Traffic Control Demo</title>
    <!-- Bootstrap Core CSS -->
	<link href="vis/vis.css" rel="stylesheet" type="text/css">
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
  	<div id="mynetwork" class="full-height col-md-8" style="border-right:1px solid #ddd"></div>
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
		<div id="log" class="col-md-12">
			<h4 class='text-muted section' style='margin-left: -15px'>Logs</h4>
			<ul class='list-group' id="log-content"></ul>
		</div>
	</div>
</div>


<script src="/statics/js/jquery.min.js" type="text/javascript"></script> 
<script src="/statics/js/bootstrap.min.js" type="text/javascript"></script> 
<script type="text/javascript" src="vis/vis.js"></script>
<script type="text/javascript" src="index.js"></script>
<script type="text/javascript" src="simulate.js"></script>
<script type="text/javascript">
	var pool_intervals 	= [];
	function createInterval(name, callback, interval) { pool_intervals.push({name: name, id: setInterval(callback, interval) }); }
	function stopInterval(name) { 
		console.log(pool_intervals);
		for(var i = 0; i < pool_intervals.length; i++) 
			if(pool_intervals[i].name === name) 
				clearInterval(pool_intervals[i].id);
	}
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
							log("Settings updated successfully!", 'success');
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
							log("Settings updated successfully!", 'success');
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

		var grid = [];
		update_info(function (info){
			log("Valid communication with server stablished!", "success");
			log("Retrieving city's information.....");
			make_request(
				data_feedback(),
				function(data) {
					if(!is_failed_data(data)) {
						info.feed = data;
						$("#log-content log").first().remove();
						log("Building city structure!");
						grid = draw_grid('mynetwork', info);
						log("City structure created successfully!", 'success');
						simulate(grid);
					}
				});
		});
	});
</script>
</body>
</html>