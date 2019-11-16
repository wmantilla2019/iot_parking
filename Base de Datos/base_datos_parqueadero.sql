-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Versión del servidor:         10.4.6-MariaDB - mariadb.org binary distribution
-- SO del servidor:              Win64
-- HeidiSQL Versión:             10.2.0.5599
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Volcando estructura de base de datos para parqueadero
CREATE DATABASE IF NOT EXISTS `parqueadero` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `parqueadero`;

-- Volcando estructura para tabla parqueadero.disponibilidad_bahia
CREATE TABLE IF NOT EXISTS `disponibilidad_bahia` (
  `bahia` int(11) DEFAULT NULL COMMENT 'Número de la bahía del parqueadero',
  `id_ocupante` varchar(50) DEFAULT NULL COMMENT 'Este es el ID de la tarjeta RFID. Llave primaria en la tabla usuario',
  `hora_estacionamiento` varchar(8) DEFAULT NULL COMMENT 'Hora a la que estaciona el vehículo',
  `fecha_estacionamiento` varchar(10) DEFAULT NULL COMMENT 'Fecha a la que estaciona el vehículo',
  `hora_salida` varchar(8) DEFAULT NULL COMMENT 'Hora de salida del vehículo de la bahía',
  `fecha_salida` varchar(10) DEFAULT NULL COMMENT 'Fecha de salida del vehículo de la bahía'
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Esta tabla contiene la informnación de diasponibilidad de las bahías del parquedaero.\r\nEsta tala mantiene el histórico de asignación de las bahías.';

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla parqueadero.estado_actual_bahia
CREATE TABLE IF NOT EXISTS `estado_actual_bahia` (
  `bahia` int(11) DEFAULT NULL COMMENT 'Número físico de la bahía',
  `estado` varchar(50) DEFAULT NULL COMMENT 'libre. asignada u ocupada'
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Esta tabla contiene el estado actual de la bahía (libre, asignada u ocupada)';

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla parqueadero.estado_estudiante
CREATE TABLE IF NOT EXISTS `estado_estudiante` (
  `id` int(11) DEFAULT NULL,
  `estado` varchar(50) DEFAULT NULL COMMENT '0 - Inactivo, 1 - Activo'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla parqueadero.programa_academico
CREATE TABLE IF NOT EXISTS `programa_academico` (
  `id` int(11) DEFAULT NULL COMMENT 'Un identificador del programa acdémico',
  `nombre` varchar(50) DEFAULT NULL COMMENT 'El nombre del programa',
  `tipo` varchar(50) DEFAULT NULL COMMENT 'Pregrado, Postgrado, Tecnólogo, Extensión, entre otros'
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Esta tabla contiene información básica del programa acdémico de la universidad';

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla parqueadero.rol_usuario
CREATE TABLE IF NOT EXISTS `rol_usuario` (
  `id` int(11) DEFAULT NULL,
  `nombre_rol` varchar(50) DEFAULT NULL COMMENT '1 - Estudiante, 2 - Adminsitrativo, 3 - Empleado, 4 - Invitado'
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Esta tabla contiene los roles de usuario';

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla parqueadero.usuario
CREATE TABLE IF NOT EXISTS `usuario` (
  `id` varchar(50) DEFAULT NULL COMMENT 'ID de la tarjeta RFID',
  `cedula` varchar(50) DEFAULT NULL COMMENT 'Cedúla de ciudadanía o extrenjería',
  `codigo_universitario` varchar(50) DEFAULT NULL COMMENT 'Código asignado por la Universidad al estudiante',
  `nombres` varchar(50) DEFAULT NULL COMMENT 'Nombres completos del estudiante',
  `apellidos` varchar(50) DEFAULT NULL COMMENT 'Apellidos del estudiante',
  `id_programa_academico` int(11) DEFAULT NULL COMMENT 'Este campo es la llave primaria en la tabla programa_academico',
  `estado_estudiante` int(11) DEFAULT NULL COMMENT '1 - Activo o 0 - Inactivo. Bloquea el acceso al parqueadero',
  `celular` varchar(50) DEFAULT NULL COMMENT 'Teléfono del estudiante',
  `email` varchar(50) DEFAULT NULL COMMENT 'Correo electrónico',
  `direccion` varchar(50) DEFAULT NULL COMMENT 'Dirección de su residencia (opcional)',
  `placa_vehiculo` varchar(50) DEFAULT NULL COMMENT 'Placa del vehiculo',
  `rol` int(11) DEFAULT NULL COMMENT '1 - Estudiante, 2 - Adminsitrativo, 3 - Empleado, 4 - Invitado'
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Contiene la información del usuario. \r\nEl usuario puede ser Estudiante, Administrativo o Invitado';

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla parqueadero.validaciones
CREATE TABLE IF NOT EXISTS `validaciones` (
  `id_usuario` varchar(50) DEFAULT NULL COMMENT 'Cedula',
  `login` varchar(50) DEFAULT NULL COMMENT 'Nombre de Usuario',
  `password` varchar(50) DEFAULT NULL COMMENT 'Contraseña'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- La exportación de datos fue deseleccionada.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
