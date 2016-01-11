function graph(div, data, data2, data3) {
	var chart = new CanvasJS.Chart(div, {
			zoomEnabled: true,
			title : {
				text : "Never Forget"
			},
			legend : {
				fontSize:14,
				fontFamily:"tahoma",
				fontColor:"Sienna",
				horizontalAlign:"right",
				verticalAlign:"top",
			},
			data : [],
			axisY:{
				title : "Temperature in F",
			},
			axisX:{
				valueFormatString: "DD/MMM/YYYY hh:mm"
			}
		});

	//	Pushing the data into the object let's you add a bunch of datasets...
	chart.options.data.push(data);
	chart.options.data.push(data2);
	chart.options.data.push(data3);
	chart.render();
}