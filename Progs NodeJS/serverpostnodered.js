var express = require('express');
var http = require('http');
var app = express();
var mysql = require('mysql');  

app.use(express.urlencoded());
app.use(express.json());

var con = mysql.createConnection({
  host: "localhost",
  user: "root",
  password: "",
  database: "parqueadero"
});

app.post('/usuario_y_bahia_libre_x_id_usuario', function(request, response){
	var sql_anidada = "SELECT bahia FROM estado_actual_bahia WHERE estado = 'libre' LIMIT 1";
	var id_usuario = request.body.id;
	var bahia_libre = 0;
    console.log('RFID: '+ id_usuario);
	
	con.query("SELECT nombres, apellidos, placa_vehiculo, (" + sql_anidada + ") AS bahia FROM usuario WHERE id ='" + id_usuario + "'", function (err, result, fields) {
		if (err) throw err;
		console.log(result);
		
		output = JSON.stringify(result); //Le da el formato JSON
		response.setHeader("Content-Type", "application/json"); //Set de la cabecera en formato JSON
		response.end(output); //Envia la cadena JSON
		
		//Consulta el número de bahía libre
		var sql_bahia_libre = "SELECT bahia FROM estado_actual_bahia WHERE estado = 'libre' LIMIT 1";
		con.query(sql_bahia_libre, function (err, result, fields) {
			if (err) throw err;
			//output = JSON.stringify(result); //Le da el formato JSON
			//console.log("La bahia libre es: " + result[0].bahia);	
			bahia_libre = result[0].bahia
			if (bahia_libre != 0) {
				//Actualiza la tabla estado_actual_bahia
				var sql = "UPDATE estado_actual_bahia SET estado = 'asignada' WHERE bahia =" + bahia_libre;
				console.log(sql);
				con.query(sql, function (err, result) { //Ejecuta la consulta
					if (err) throw err;
					console.log("E1 registro fue actualizado");
				});
				//INSERTA datos a la tabla disponibilidad_bahia
				var sql = "INSERT INTO disponibilidad_bahia (bahia, id_ocupante) VALUES (" +  bahia_libre + ", '" + id_usuario + "')";
				console.log(sql);
				con.query(sql, function (err, result) { //Ejecuta la consulta
					if (err) throw err;
					console.log("E1 registro fue actualizado");
				});
			}
		});
	 });	
});

//Esta función se ejecuta cuando un vehículo se estaciona en una bahía y la cambia a estado ocupado
//Se almacenan la hora y la fecha de entrada
app.post('/actualiza_estado_bahia', function(request, response){
	var bahia = request.body.bahia;
	var estado_bahia = request.body.estado;
	let fechaactual= new Date();	
	var fecha = (fechaactual.getDate() + "/" + (fechaactual.getMonth() +1) + "/" + fechaactual.getFullYear());
	var hora = (fechaactual.getHours() + ":" + fechaactual.getMinutes() + ":" + fechaactual.getSeconds());
	
	var sql = "UPDATE estado_actual_bahia  SET estado = '" + estado_bahia + "' WHERE bahia=" + bahia;
    console.log(sql);
	
	con.query(sql, function (err, result, fields) {
		if (err) throw err;
		//console.log(result);		
		console.log("Registro actualizado.");
		//response.status(200).send("Ok");
	});

	var sql = "UPDATE disponibilidad_bahia SET hora_estacionamiento = '" + hora  + "', fecha_estacionamiento = '" + fecha + "' WHERE bahia=" + bahia +
				" AND hora_estacionamiento IS NULL AND fecha_estacionamiento IS NULL";;
    console.log(sql);
	
	con.query(sql, function (err, result, fields) {
		if (err) throw err;
		//console.log(result);		
		console.log("Registro actualizado.");
	});
	
	response.status(200).send("Ok"); 
});

//Esta función se ejecuta cuando el vehículo que estuvo estacionado por cierto tiempo, ya sale del parqueadero
//Creada el 12 de Noviembre de 2019 - WMA. En prueba
app.post('/actualiza_hora_salida_bahia', function(request, response){
	
	var bahia = request.body.bahia;
	var estado_bahia = request.body.estado;
	let fechaactual= new Date();	
	var fecha = (fechaactual.getDate() + "/" + (fechaactual.getMonth() +1) + "/" + fechaactual.getFullYear());
	var hora = (fechaactual.getHours() + ":" + fechaactual.getMinutes() + ":" + fechaactual.getSeconds());
	
	var sql = "UPDATE estado_actual_bahia  SET estado = '" + estado_bahia + "' WHERE bahia=" + bahia;
    console.log(sql);
	
	con.query(sql, function (err, result, fields) {
		if (err) throw err;
		//console.log(result);		
		console.log("Registro actualizado.");
		//response.status(200).send("Ok");
	});

	var sql = "UPDATE disponibilidad_bahia SET hora_salida = '" + hora  + "', fecha_salida = '" + fecha + "' WHERE bahia=" + bahia + 			
				" AND hora_salida IS NULL AND fecha_salida IS NULL";
    console.log(sql);
	
	con.query(sql, function (err, result, fields) {
		if (err) throw err;
		//console.log(result);		
		console.log("Registro actualizado.");
	});
	
	response.status(200).send("Ok"); 
});

app.get('/usuario_y_bahia_libre_x_id_usuario', function(request, response) { //Obtiene ... y los devuelve mediante get
	var sql_anidada = "SELECT bahia FROM estado_actual_bahia WHERE estado = 'libre' LIMIT 1";
	var id_usuario = request.body.id;
	
    console.log('RFID: '+ id_usuario);
	
	con.query("SELECT nombres, apellidos, placa_vehiculo, (" + sql_anidada + ") AS bahia FROM usuario WHERE id ='" + id_usuario + "'", function (err, result, fields) {
	//con.query("SELECT bahia, estado FROM estado_actual_bahia WHERE estado = 'libre' LIMIT 1", function (err, result, fields) {
	//con.query("SELECT * FROM datos", function (err, result, fields) {	
		if (err) throw err;
		console.log(result);
		
		output = JSON.stringify(result); //Le da el formato JSON
		response.setHeader("Content-Type", "application/json"); //Set de la cabecera en formato JSON
		response.end(output); //Envia la cadena JSON
	});
});

/*app.post('/uno', (req, res) => {
  res.status(200).send("Bienvenido a API REST")
})*/

http.createServer(app).listen(8001, "0.0.0.0", () => {
  console.log('Servidor iniciado en http://localhost:8001');
});