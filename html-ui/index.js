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
		beforeSend: function() { log("<span class='trans-log'>Talking to server...</span>"); }
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
function makeArray(d1, d2) { var arr = new Array(d1), i, l; for(i = 0, l = d1; i < l; i++) arr[i] = new Array(d2); return arr; };
function convert_feed(grid_size, feed) {
	if(feed.clusters === undefined || !feed.clusters.length) return [];
	c = [];
	feed = feed.clusters;
	gs = makeArray(grid_size[0], grid_size[1]);
	for (var i = 0; i < feed.length; i++) c = c.concat(feed[i]);
	for (var i = 0; i < c.length; i++) {
		var s = c[i];
		var loc = (/\[\s*(\d+)\s*,\s*(\d+)\s*\].*-(\w)/g).exec(s.name);
		var h = parseInt(loc[1]), w = parseInt(loc[2]);
		s.loc = [h, w];
		s.dir = loc[3];
		if(gs[h][w] === undefined) gs[h][w] = [];
		gs[h][w].push(s);
	}
	return gs;
};
function draw_grid(elem, grid) {
	if(grid.grid_size === undefined || grid.grid_size.length !== 2) throw "Invalid grid info!";

	var nodes = []; 
	var edges = [];

	var h = grid.grid_size[0], w = grid.grid_size[1];
	grid.feed = convert_feed(grid.grid_size, grid.feed);
	var coor2id = function(x, y, x_max) { return x * x_max + y; };
	var nodes_id = makeArray(h, w);
	for(var i = 0, n = 0; i < h; i++) { 
		for(var j = 0; j < w; j++) {
			var node = {
				id: n,
				value: 1,
				title: "["+i+","+j+"]",
				streets: (grid.feed[i] === undefined || grid.feed[i][j] === undefined ? [] : grid.feed[i][j]),
				x: i,
				y: j
			};
			if(j != 0) {
				var prev = nodes[nodes_id[i][j-1]];
				var prev_s = prev.streets;
				for (var k = 0; k < prev_s.length; k++) {
					if(prev_s[k].dir === 'R') {
						node.x = prev.x + 300 * Math.log(prev_s[k].length);
						break;
					}
				}
			}
			if(i != 0) {
				var prev = nodes[nodes_id[i-1][j]];
				var prev_s = prev.streets;
				for (var k = 0; k < prev_s.length; k++) {
					if(prev_s[k].dir === 'D') {
						node.y = prev.y + 300 * Math.log(prev_s[k].length); 
						break;
					}
				}
			}
			nodes_id[i][j] = n++;
			nodes.push(node);
		}
	}

	for(var i = 0, n = 0; i < h; i++) { 
		for(var j = 0; j < w; j++) {
			if(grid.feed[i] === undefined) continue;
			var s = grid.feed[i][j];
			for(var k = 0; s !== undefined && k < s.length; k++) {
				var t = [i + 1, j];
				if(s[k].dir == "R") t = [i, j + 1];
				nodes[nodes_id[i][j]].value += s[k].capacity;
				edges.push({from: nodes_id[i][j], to: nodes_id[t[0]][t[1]], value: s[k].capacity, tag: s[k], color: "#D2E5FF"});
			}
		}
	}
	var options = {
		nodes: {
			fixed: true,
			shape: 'dot',
			scaling:{
				min: 1,
				max: 300
			}
		},
		edges: {
			scaling:{
				min: 1,
				max: 300
			}
		},
		layout: { randomSeed:0 }
	};
	var data = {
		nodes: new vis.DataSet(nodes),
		edges: new vis.DataSet(edges)
	};
	return { network: new vis.Network(document.getElementById(elem), data, options), nodes: data.nodes, edges: data.edges };
};
function update_info(callback) {
	var do_draw = typeof do_draw !== 'undefined' ?  do_draw : false;
	make_request(data_get_info(), function(info) {
		if(!is_failed_data(info, true)) {
			$("#grid-height").val(info.grid_size[0]).data('oldval', info.grid_size[0]);
			$("#grid-width").val(info.grid_size[1]).data('oldval', info.grid_size[1]);
			$("#grid-car-no").val(info.car_count).data('oldval', info.car_count);
			$("#grid-time-step").val(Math.round(info.time_step*100)/100).data('oldval', info.time_step);
			$("#grid-cluster-delay").val(Math.round(info.cluster_delay*100)/100).data('oldval', info.cluster_delay);
			$("#grid-state-"+info.flow).prop("checked", true);
			if(callback !== undefined) callback(info);
		}
	});
};
