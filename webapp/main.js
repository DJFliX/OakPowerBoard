var buttons = [
    { name: "Up", glyph: "glyphicon-menu-up"},
    { name: "Right", glyph: "glyphicon-menu-right"},
    { name: "Left", glyph: "glyphicon-menu-left"},
    { name: "Down", glyph: "glyphicon-menu-down"}
];

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

function addCss(id, url) {
    var head = document.getElementsByTagName("head")[0];
    if (!document.getElementById(id))
    {
        var head  = document.getElementsByTagName('head')[0];
        var link  = document.createElement('link');
        link.id   = cssId;
        link.rel  = 'stylesheet';
        link.type = 'text/css';
        link.href = url;
        link.media = 'all';
        head.appendChild(link);
    }
}

function add(button_index) {
    //Create an input type dynamically.   
    var element = document.createElement("input");
    //Assign different attributes to the element. 
    element.type = 'button';
    element.class = 'btn btn-info';
    var span = document.createElement("span");
    span.class = "glyphicon " +  buttons[button_index].glyph;
    element.appendChild(span);
    element.name = 'button' + button_index;  // And the name too?
    element.onclick = function() { // Note this is a function
        httpGetAsync("api/b" + button_index, button_callback)
    };
    document.getElementById("bc").appendChild(element);
}

function initialize() {
    addCss('bootstrap', '//maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css');
	add(1);
	add(2);
	add(3);
	add(4);
	document.write('Successfully loaded content from CDN');
}

initialize();