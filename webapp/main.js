var buttons = ["glyphicon-menu-up", "glyphicon-menu-right", "glyphicon-menu-left", "glyphicon-menu-down"];

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
        link.id   = id;
        link.rel  = 'stylesheet';
        link.type = 'text/css';
        link.href = url;
        link.media = 'all';
        head.appendChild(link);
    }
}

function createGlyphButton(button_index) {
    //Create an input type dynamically.   
    var element = document.createElement("button");
    //Assign different attributes to the element. 
    element.setAttribute('class', 'btn btn-info');
    var span = document.createElement("span");
    span.setAttribute('class', "glyphicon " + buttons[button_index - 1]);
    element.appendChild(span);
    element.name = 'button' + button_index;  // And the name too?
    element.onclick = function() { // Note this is a function
        httpGetAsync("api/b" + button_index, button_callback)
    };
    return element;
}

function createInputGroup(id, contents) {
	var divElement = document.createElement("div");
	divElement.setAttribute('class', 'input-group');
	divElement.setAttribute('id', id);
	for(var i = 0; i < contents.length; i++) {
		divElement.appendChild(contents[i]);
	}
	return divElement;
}

function createTextField(id, placeholder, hasAddon) {
	var inputElement = document.createElement("input");
	inputElement.setAttribute('type', 'text');
	inputElement.setAttribute('id', id);
	inputElement.setAttribute('class', 'form-control');
	inputElement.setAttribute('placeholder', placeholder);
	if(hasAddon) inputElement.setAttribute('aria-describedby', id + '-addon');
	return inputElement;
}

function createTextFieldAddon(id, contents) {
	var span = document.createElement("span");
	span.setAttribute('class', 'input-group-addon');
	span.setAttribute('id', id);
	span.textContent = contents;
    return span;
}

function createLabel(target_element, content) {
	var label = document.createElement("label");
	label.setAttribute('for', target_element);
	label.textContent = content;
	return label;
}

function createKwhForm() {
	var form = document.createElement("form");
	var formGroup = document.createElement("div");
	formGroup.setAttribute("class", "form-group");
	formGroup.appendChild(createLabel("kwh-input", "KWh preset"));

	var inputGroup = createInputGroup("kwh-input-addon", [
		createTextField("kwh-input", "12345.00", true), 
		createTextFieldAddon('kwh-input-addon', 'KWh')
	]);

	formGroup.appendChild(inputGroup);

	form.appendChild(formGroup);
	return form;
}

function getStatus(callback) {
    httpGetAsync('api/status', callback);
}

function processStatus(json) {
    var parsedStatus = JSON.parse(json);
    document.getElementById('kwh-input').value = parsedStatus.total;
}

function initialize() {
    addCss('bootstrap', 'https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css');
    var dstElement = document.getElementById("bc");
	dstElement.appendChild(createGlyphButton(1));
	dstElement.appendChild(createGlyphButton(2));
	dstElement.appendChild(createGlyphButton(3));
	dstElement.appendChild(createGlyphButton(4));
	dstElement.appendChild(createKwhForm());
    getStatus(processStatus);
}

initialize();