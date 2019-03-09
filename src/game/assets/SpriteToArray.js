/*
--- Information about the sprites (this program only fixes a majority of the manual work) ---

The game uses arrays for it's sprites. The .pixil file is from an online website that I used to create all of the existing sprites (https://www.pixilart.com/draw).
This program is a quick CLI program to convert image files to text arrays using "node SpriteToArray.js sprite.png".

Most of it still requires tweaking to render in the game.

Be sure to have node installed and install the dependencies by running "npm install" in the main directory.
*/


const pngToMatrix = require("png-to-matrix");

let imageString = "sint32 spriteName[] = {\n";

function rgbToHex(R,G,B) {
	return toHex(R)+toHex(G)+toHex(B);
}

function toHex(n) {
	n = parseInt(n,10);
	if (isNaN(n)) return "00";
	n = Math.max(0,Math.min(n,255));
	return "0123456789ABCDEF".charAt((n-n%16)/16) + "0123456789ABCDEF".charAt(n%16);
}

pngToMatrix("./"+process.argv[2], (matrix) => {
	for (let i=0; i<matrix.length; i++) {
		let row = "	";
		for (let j=0; j<matrix[i].length; j++) {
			row += "0x"+rgbToHex(matrix[i][j].r, matrix[i][j].g, matrix[i][j].b)+"00, ";
		}
		imageString += row + "\n"
	}
	imageString += "};";
	console.log(imageString);
});