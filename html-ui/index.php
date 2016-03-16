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
<body>
<script src="/statics/js/jquery.min.js" type="text/javascript"></script> 
<script src="/statics/js/bootstrap.min.js" type="text/javascript"></script> 
<script type="text/javascript" src="vis/vis.js"></script>
<link href="vis/vis.css" rel="stylesheet" type="text/css" />

  <style type="text/css">
    * {
      font: 50pt arial;
    }
  </style>
    
<script type="text/javascript">
	function echo(str) {
		document.write(str);
	}
	$(document).ready(function(){
		data = {
			status: "check",
			last_cal: 'FUCKU'
		};
    	$.ajax({
			url: "http://127.0.0.1:2004",
			method: "POST",
			data: JSON.stringify(data), 
			cache: false,
			success: function(data) {
				console.log("FUCK");
				console.log(data);
				console.log([234, 5, 5, 5])
			}
		});
		return;

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
	      var data = {
	        nodes: nodes,
	        edges: edges
	      };
	      var z = []
	      for(var i = 0; i < 10; i++) {
	      	nodes[i].x = i * 100;
	      	nodes[i].y = i * 100;
	      }
	      var options = {
	        nodes: {
        		fixed: true,
				shape: 'dot',
				scaling:{
					label: {
						min:1,
						max:10
					}
				}
	        },
	        layout: {randomSeed:0}
	      };
	      network = new vis.Network(container, data, options);
	    }
	    $("#mynetwork").css({"height": $(document).height(), "width": $(document).width()});
	    draw();
	});
</script>
<div id="mynetwork" style="border:1px solid #000"></div>
</body>
</html>