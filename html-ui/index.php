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
<script type="text/javascript">
	function echo(str) {
		document.write(str);
	}
	$(document).ready(function(){
    	// Create SocketIO instance, connect
    	$.ajax({
			url: "http://127.0.0.1:2004",
			method: "POST",
			data: { id : "122" },
			// dataType: "html", 
			success: function(data) {
				console.log("FUCK");
				console.log(data);
				console.log([234, 5, 5, 5])
			}
		});
	});
</script>
<div class="container" style="padding-top: 100px;">
</div>
</body>
</html>