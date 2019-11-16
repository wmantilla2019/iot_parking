SELECT bahia, nombres, apellidos, placa_vehiculo, hora_estacionamiento, fecha_estacionamiento, hora_Salida, fecha_Salida
FROM parqueadero.disponibilidad_bahia, parqueadero.usuario 
WHERE parqueadero.disponibilidad_bahia.id_ocupante = parqueadero.usuario.id 
AND parqueadero.disponibilidad_bahia.bahia = 1 