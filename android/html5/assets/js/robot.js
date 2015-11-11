    $(document).ready(function()
    {

	$('#pointsx').change(function() {
      		x_send();
	});		
	$('#pointsy').change(function() {
      		y_send();
	});	
	/////////
//	$( "#c_front" ).addEventListener("touchstart", car_cmd("b1"), false);
	var bt_car;
	bt_car=document.getElementById("c_front");
	bt_car.addEventListener('touchstart', function(e) {car_cmd("b1");}, false);
	bt_car.addEventListener('touchend', function(e) {car_cmd("b0");}, false);
	bt_car=document.getElementById("c_right");
	bt_car.addEventListener('touchstart', function(e) {car_cmd("r2");car_cmd("l-2");}, false);
	bt_car.addEventListener('touchend', function(e) {car_cmd("b0");}, false);
	bt_car=document.getElementById("c_back");
	bt_car.addEventListener('touchstart', function(e) {car_cmd("b-3");}, false);
	bt_car.addEventListener('touchend', function(e) {car_cmd("b0");}, false);
	bt_car=document.getElementById("c_left");
	bt_car.addEventListener('touchstart', function(e) {car_cmd("l2");car_cmd("r-2");}, false);
	bt_car.addEventListener('touchend', function(e) {car_cmd("b0");}, false);

	 return;

	//////////

    });


