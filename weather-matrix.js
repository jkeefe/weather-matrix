var request = require('request'),
    moment = require('moment');
    keys = require('../api_keys/weather_matrix_keys');
    
    // that last file holds my values for
    // SPARK_DEVICE_ID      (use keys.SPARK_DEVICE_ID)
    // SPARK_ACCESS_TOKEN   (use keys.SPARK_ACCESS_TOKEN)
    // FORECAST_IO_API_KEY  (use keys.FORECAST_IO_API_KEY)
    

    
// using latitude and longitude of central park
var lat = 40.795, 
    lon = -73.956;

// configure the request to forecast.io fo
var options = {
    url: 'https://api.forecast.io/forecast/'+ keys.FORECAST_IO_API_KEY +'/' + lat + ',' + lon,
    method: 'GET'
};

// Start the request to forecast.io
request(options, function (error, response, body){
    
    if (error || response.statusCode != 200) {
        
        // something went wrong getting forecast
        console.log("Forecast fetching error: ", error);
    
    } else {
    
        // success!
        var weather = JSON.parse(body);
        
        var forecast;
        var high;
        var icon;
        var send_string;
        
        // NOTE: The next section determines the time based on the
        // forecast city. Since this is static here -- the forecast lat/lon
        // is included above -- I could skip this and go off the server's time.
        // But I've written this code before, and including it makes it more
        // universal for anyone else.
        
        // What time is it!?!
        // Here current_local_time will be set to the time at the forecast location
        var current_local_time = moment.unix(weather.currently.time);
    
            // adjust the offset based on the one provided by forecast, 
            // so we are using times local to the forecast point.
            current_local_time = current_local_time.utcOffset(weather.offset);
            
            // // check it by uncommenting the next line
            // console.log(current_local_time.format("dddd, MMMM Do YYYY, h:mm:ss a"));
        
        // use today's forecast data[0] if it's before noon in the forecast location
        // use tomorrow's forecast data[1] if it's after noon in the forecast location
        if (current_local_time.hour() < 12) {
            forecast = weather.daily.data[0];
        } else {
            forecast = weather.daily.data[1];
        }
        
        // extract the high temperature from forecast.io
        // first get the number as a string, (base 10)
        high = Math.round(forecast.temperatureMax).toString(10); 
        
        // then tack three spaces onto the end
        high = high + "   ";
        
        // then take the first three characters of the combination,
        // which is the temp (and a maybe a minus sign) padded at the
        // end by spaces. So "-1  " or "20 " or "101."
        high = high.substring(0,3);
        
        // extract the icon from forecast.io
        // ie: "partly-cloudy-day" or "snow"
        icon = forecast.icon;

        // combine them to send
        // ie: "34 snow"
        send_string = high + icon;

        // send it to the spark core!
        sendToSpark(send_string);
        
    }
    
});

// Send data to a spark core
function sendToSpark(data) {
	
	// set the headers
	var headers = {
		'Content-Type': 'application/x-www-form-urlencoded'
	};
	
	// configure the request
	var options = {
		url: 'https://api.spark.io/v1/devices/' + keys.SPARK_DEVICE_ID + '/forecast',
		method: 'POST',
		headers: headers,
		form: {
			'access_token': keys.SPARK_ACCESS_TOKEN,
			'params': data
		}
	};
	
	// Make the request
	request(options, function (error, response, body) {
	    if (!error && response.statusCode == 200) {
	        // Print out the response body
	        console.log(body);
	    } else {
	        console.log(error);
	    }
	});
	
	
}
