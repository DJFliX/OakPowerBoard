function httpGetAsync(theUrl, callback)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.responseText);
    }
    xmlHttp.open("GET", theUrl, true); // true for asynchronous 
    xmlHttp.send(null);
}

function button_callback(err, rsp) {
	console.log(err);
	console.log(rsp);
}

function add(button_index) {
    //Create an input type dynamically.   
    var element = document.createElement("input");
    //Assign different attributes to the element. 
    element.type = 'button';
    element.value = 'B' + button_index + ' (Up)'; // Really? You want the default value to be the type string?
    element.name = 'button' + button_index;  // And the name too?
    element.onclick = function() { // Note this is a function
        httpGetAsync("api/b" + button_index, button_callback)
    };

    var foo = document.getElementById("bc");
    //Append the element in page (in span).  
    foo.appendChild(element);
}

function initialize() {
	add(1);
	add(2);
	add(3);
	add(4);
	document.write('Successfully loaded content from CDN');
}

initialize();