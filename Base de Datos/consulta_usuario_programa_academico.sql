select usuario.id, nombres, apellidos, nombre as programa, rol_usuario.nombre_rol AS rolusuario, estado_estudiante.estado, placa_vehiculo 
from usuario, programa_academico, rol_usuario, estado_estudiante 
where programa_academico.id = usuario.id_programa_academico AND usuario.rol = rol_usuario.id AND usuario.estado_estudiante=estado_estudiante.id