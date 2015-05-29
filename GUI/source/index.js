var cmd = require('child_process').exec;

function minifyRegex (regex1, regex2) {
	cmd("./helper/simplify " + regex1 + " " + regex2, function (error, stdout, stderr) {
		document.getElementById("DFAToRegex").innerText = stdout;
	});
}