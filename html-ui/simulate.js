function rand(max, min) { var min = min === undefined ? 0 : min; return min + Math.random() * (max - min);  }
function rand_index(max, min) { return Math.floor(rand(max, min)); }
function rand_color() { var letters = '0123456789ABCDEF'.split(''); var color = '#'; for (var i = 0; i < 6; i++ ) color += letters[Math.floor(Math.random() * 16)]; return color; }
 
function simulate(grid) {
	make_request(
		data_feedback(),
		function(data) {
			if(!is_failed_data(data)) {
				// convert the feed
				var feed = convert_feed(grid.size, data);
				// update the streets' traffic weight
				for(var i = 0; i < grid.size[0]; i++) {
					for(var j = 0; j < grid.size[0]; j++) {
						var node_val = 0
						var st = feed[i][j];
						for(var k = 0; st !== undefined && k < st.length; k++) {
							var tw = st[k].size.reduce((a, b) => a + b, 0);
							grid.updateEdge(get_street_coord(st[k]), {value: tw, title: ['['+st[k].size.toString()+']', 'tw: '+ tw].join(' ')});
						}
					}
				}
				// update the joints' traffic weight
				for(var i = 0; i < grid.size[0]; i++) {
					for(var j = 0; j < grid.size[0]; j++) {
						var node = grid.getNode([i,j]);
						var st = node.streets;
						var val = 0;
						for(var k = 0; st !== undefined && k < st.length; k++) 
							val += st[k].size.reduce((a, b) => a + b, 0);
						grid.updateNode([i,j], {value: val, title: ['['+[i,j].toString()+']', 'tw: '+ val].join(' ')});
					}
				}
				setTimeout(simulate, 1000, grid);
			}
		}, false);
}