var cmd = require('child_process').exec;

function minifyRegex (regex1, regex2) {
	cmd("./helper/simplify " + regex1 + " " + regex2, function (error, stdout, stderr) {
		document.getElementById("usedregex").innerText = regex1;

		if (regex2 !== "") {
			document.getElementById("usedregex").innerText += " producted with " + regex2;
		}

		document.getElementById("DFAToRegex").innerText = stdout;

		rerenderGraphs();
	});
}

function rerenderGraphs () {
	var graphFiles = [{
		name: "step1",
		file: "step1.gv"
	}, {
		name: "step2",
		file: "step2.gv"
	}, {
		name: "step3",
		file: "step3.gv"
	}];

	for (var k = 0; k < graphFiles.length; k++) {
		getGraph(graphFiles[k]);
	}
}

function getGraph (graphSpecs) {
	var request = new XMLHttpRequest();
	request.open("GET", graphSpecs.file + "?t=" + Date.now(), true);
	request.addEventListener("readystatechange", function () {
		if (request.readyState == 4) {
			// Parse the graph
			var svg = document.getElementById(graphSpecs.name);
			svg.innerHTML = Viz(request.responseText, "svg");

			// Resize the svg drawing
	        var bbox = svg.getBBox();

	        svg.setAttribute("viewBox", "0 0 " + (bbox.width + 40) + " " + (bbox.height + 40));
	        //svg.style.width =  bbox.width + 40.0 + "px";
	        //svg.style.height = bbox.height + 40.0 + "px";
		}
	});
	request.send();
}

var lastFive = "     ";
document.addEventListener("keypress", function (event) {
	lastFive += String.fromCharCode(event.keyCode);
	lastFive = lastFive.slice(-5);

	if (lastFive.indexOf("dev") == 2) {
		require('nw.gui').Window.get().showDevTools()
	}
});