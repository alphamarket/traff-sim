// a loging interface
function log(data, type) {
	$("#log-content")
		.prepend('<log class="list-group-item list-group-item-'+type+'"><i class="fa fa-angle-double-right"></i> '+data+'</li>');
};
// get data for help command 
function data_help() 
	{ return {op: "help"}; };
// get data for feedback command
function data_feedback() 
	{ return { op: "feedback" }; };
// get data for information retrival command
function data_get_info() 
	{ return { op: "get_info" }; };
// get data for grid initialization command
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
// get data for setting command
function data_setting(key, value) { 
	var key = typeof key !== 'undefined' ?  key : [];
	var value = typeof value !== 'undefined' ?  value : [];
	if(key.length !== value.length) throw "invalid input!"; 
	return { op: "setting", set: { key: key, value: value } }; 
};
// makes a request to server
// on successful result the callback will get fired
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
// validates if the incomming data from server contains a failure message?
function is_failed_data(data, do_log) { 
	var do_log = typeof do_log !== 'undefined' ?  do_log : false;
	if(data.result === undefined || data.result !== "failed") return false;
	if(do_log) log("<b>Error " + data.code + ": " + data.detail + "</b>", "danger"); 
	return true;
};
// create a 2D array
function makeArray(d1, d2) { var arr = new Array(d1), i, l; for(i = 0, l = d1; i < l; i++) arr[i] = new Array(d2); return arr; };
// converts a feed instance into processable structure
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
// draw a grid in a element based on the grid information passed on
function draw_grid(elem, grid) {
	if(grid.grid_size === undefined || grid.grid_size.length !== 2) throw "Invalid grid info!";

	var nodes = []; 
	var edges = [];
	
	var h = grid.grid_size[0]
	var w = grid.grid_size[1];

	var nodes_id = makeArray(h, w);

	grid.feed = convert_feed(grid.grid_size, grid.feed);

	// create nodes
	for(var i = 0, n = 0; i < h; i++) { 
		for(var j = 0; j < w; j++) {
			// define a node
			var node = {
				id: n,
				value: 1,
				title: "["+i+","+j+"]",
				streets: (grid.feed[i] === undefined || grid.feed[i][j] === undefined ? [] : grid.feed[i][j]),
				x: i,
				y: j
			};
			// make node's distances relatively similar to the simulator's distance
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
			// add the node
			nodes_id[i][j] = n++;
			nodes.push(node);
		}
	}
	// create edges
	for(var i = 0, n = 0; i < h; i++) { 
		for(var j = 0; j < w; j++) {
			// a fail-safe for in case of [h: 1, w: 1]
			if(grid.feed[i] === undefined) continue;
			// fetch the streets bound to this point
			var s = grid.feed[i][j];
			nodes[nodes_id[i][j]].edges = [];
			// foreach street => create an edges
			for(var k = 0; s !== undefined && k < s.length; k++) {
				// the tail's join't location to bound
				var t = [i + 1, j];
				if(s[k].dir == "R") t = [i, j + 1];
				// increment the traffic weight of current joint by current street's traffic weight  
				nodes[nodes_id[i][j]].value += s[k].traffic_weight;
				// create the edge
				edges.push({from: nodes_id[i][j], to: nodes_id[t[0]][t[1]], value: s[k].traffic_weight, title: s[k].traffic_weight, tag: s[k], color: "#D2E5FF"});
				// make a reference to the created edge to be accessed via the node instance 
				nodes[nodes_id[i][j]].edges.push(edges[edges.length-1]);
			}
		}
	}
	// define options
	var options = {
		nodes: {
			fixed: true,
			shape: 'square',
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
	// create a databse nodes/edges, so they can get edited later
	var data = {
		nodes: new vis.DataSet(nodes),
		edges: new vis.DataSet(edges)
	};
	// return the the grid object
	return { 
		network: new vis.Network(document.getElementById(elem), data, options), 
		nodes: data.nodes, 
		edges: data.edges, 
		nodes_id: nodes_id,
		size: [h, w],
		getNodeID: function(coord) { 
			if(coord.length !== 2) throw "the `coord` need to be a 2D coordination instance, i.e [x,y]."; 
			return this.nodes_id[coord[0]][coord[1]]; 
		},
		getEdgeID: function(coord) {
			if(coord.length !== 3) throw "the `coord` need to be a 3D coordination + direction instance, i.e [x,y,('R','D')].";
			// get the target node
			var node = this.nodes.get(this.getNodeID(coord.slice(0,2)));
			// iterate over the node's inbound edges
			for(var i = 0; i < node.edges.length; i++)
				// if matched with the direction passed?
				if(node.edges[i].tag.dir === coord[2])
					// update it
					return node.edges[i].id;
			// not found!
			return null;
		},
		getNode: function(coord) { return this.nodes.get(this.getNodeID(coord)); },
		getEdge: function(coord) { return this.edges.get(this.getEdgeID(coord)); },
		updateNode: function(coord, opt) {
			// compute the id
			var set = { id:  this.getNodeID(coord) };
			// extent the setting
			jQuery.extend(set, opt);
			// update it
			return this.nodes.update(set);	
		},
		updateEdge: function(coord, opt) {
			// compute the id
			var set = { id:  this.getEdgeID(coord) };
			// extent the setting
			jQuery.extend(set, opt);
			// update it
			return this.edges.update(set);
		}
	};
};
// updates the info values
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
