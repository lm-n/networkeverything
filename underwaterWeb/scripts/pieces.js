//Check if the data is ready
	if (wataData.ready){
		console.log("Data is ready!");
		console.log("Level: " + wataData.number);
    	console.log("Time: " + wataData.at.substring(11,19));
		if (wataData.number === 0){
			wataData.ready = false;
		}
		else{
			//Toggle the booleans
			wataData.ready = false;
			wataData.animate = true;
		}
	}