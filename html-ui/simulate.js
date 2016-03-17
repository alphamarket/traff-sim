function rand(max, min) { var min = min === undefined ? 0 : min; return min + Math.random() * (max - min);  }
function rand_index(max, min) { return Math.floor(rand(max, min)); }
function rand_color() { var letters = '0123456789ABCDEF'.split(''); var color = '#'; for (var i = 0; i < 6; i++ ) color += letters[Math.floor(Math.random() * 16)]; return color; }

function simulate(grid) {
	make_request(
		data_feedback(),
		function(data) {
			if(!is_failed_data(data)) {
				var clusters = data.clusters;
				for(var c = 0; c < clusters.length; c++) {
					for(var s = 0; s < clusters[c].length; s++) {
						var st = clusters[c][s];
						var cst = get_street_coord(st.name);
						grid.updateEdge(cst, {value: st.size.reduce((a, b) => a + b, 0)});
					}
				}
			}
		}, false);
	setTimeout(simulate, 1000, grid);
}