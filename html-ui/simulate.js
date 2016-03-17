function rand(max, min) { var min = min === undefined ? 0 : min; return min + Math.random() * (max - min);  }
function rand_index(max, min) { return Math.floor(rand(max, min)); }
function rand_color() { var letters = '0123456789ABCDEF'.split(''); var color = '#'; for (var i = 0; i < 6; i++ ) color += letters[Math.floor(Math.random() * 16)]; return color; }

function simulate(grid) {
	var id = rand_index(grid.nodes.length);
	grid.nodes.update({id: id, color: rand_color(), value: rand(10)});
	grid.updateEdge([rand_index(grid.size[0]), rand_index(grid.size[1]), rand(1) < 0.5 ? 'R' : 'D' ], {color: rand_color(), value: rand(10)});
	setTimeout(simulate, 1000, grid);
}