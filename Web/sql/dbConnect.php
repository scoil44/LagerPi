<?php

static $servername = "";
static $username = "";
static $password = "";
static $dbname = "";

$arrTemps = array();

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

function closeDB() {
	global $conn;
	mysqli_close($conn);
}

function getCommands() {
	// Access global connection variables
	global $servername, $username, $password, $dbname, $conn;

	// $sql = "SELECT * FROM `Command_Log`";
	$sql = "SELECT * FROM `Command_Log` ORDER BY `Command_Log`.`Time Stamp` DESC LIMIT 0, 20 ";
	$result = mysqli_query($conn, $sql);

	if (mysqli_num_rows($result) > 0) {
	    // output data of each row
	    while($row = mysqli_fetch_assoc($result)) {
	 		echo "<tr" . (($row["Result"]=="FAIL")?" class=\"bg-danger\"":"") . "><td>" . $row["Time Stamp"] . "</td><td>" . $row["Action"] . "</td><td>" . $row["Result"] . "</td><td>" . $row["Source"] . "</td></tr>";
	    }
	} else {
	    echo "Command Table Empty<br/>(Maybe the logs were recently cleared?)";
	}
}

function getDeviceStatus() {
	// Access global connection variables
	global $servername, $username, $password, $dbname, $conn;

	$sql = "SELECT * FROM `Device_Status`";
	$result = mysqli_query($conn, $sql);

	if (mysqli_num_rows($result) > 0) {
	    // output data of each row
	    while($row = mysqli_fetch_assoc($result)) {
	 		echo "<tr><td>" . $row["GPIO"] . "</td><td>" . $row["Device"] . "</td><td>" . $row["ID"] . "</td>";
	 		echo "<td class=\"bg-" . (empty($row["State"])?"danger":"success") . "\">" . (empty($row["State"])?"Off":"On") . "</td></tr>";
	    }
	} else {
	    echo "0 results";
	}
}

function getConfig() {
	// Access global connection variables
	global $servername, $username, $password, $dbname, $conn;

	$sql = "SELECT `Device`, `ID`, `State` FROM `Device_Status` WHERE `Device` != \"Sensor\"";
	$result = mysqli_query($conn, $sql);

	if (mysqli_num_rows($result) > 0) {
	    // output data of each row
	    while($row = mysqli_fetch_assoc($result)) {
	 		echo "<tr><td>" . $row["Device"] . "</td><td>" . $row["ID"] . "</td>";
	 		echo "<td class=\"bg-" . (empty($row["State"])?"danger":"success") . "\">" . (empty($row["State"])?"Off":"On") . "</td></tr>";
	    }
	} else {
	    echo "0 results";
	}
}

function getBeerTemperatures($sensor) {
	// Access global connection variables
	global $servername, $username, $password, $dbname, $conn, $arrTemps;
	unset($arrTemps);
	//	Get the beer's temperature
	$sql = "SELECT * FROM `Sensor_Log` WHERE `Sensor` = " . $sensor;
	$result = mysqli_query($conn, $sql);
	if (mysqli_num_rows($result) > 0) {
	    // output data of each row
	    while($row = mysqli_fetch_assoc($result)) {
	    	$arrTemps[] = ["x" => (strtotime($row["Time Stamp"]) * 1000),"y" => floatval($row["Temp"])];
		}
		print_r(json_encode(["type" => "area","name" => "BEERX","legendText" => "Never Forget","markerType" => "none","xValueType" => "dateTime","dataPoints" => $arrTemps]));
	}
}

function getTemperatureSetting() {
	// Access global connection variables
	global $servername, $username, $password, $dbname, $conn;

	$sql = "SELECT `Product Temp` FROM `Temperature_Vars`";
	$result = mysqli_query($conn, $sql);

	if (mysqli_num_rows($result) > 0) {
		$row = mysqli_fetch_assoc($result);
		echo $row["Product Temp"];
	} else {
	    echo "Temperature Error";
	}
}

function setTemperatureSetting($temperature) {
	// Access global connection variables
	global $servername, $username, $password, $dbname, $conn;

	$sql = "UPDATE `Temperature_Vars` SET `Product Temp` = " . $temperature;
	$result = mysqli_query($conn, $sql);
}

function getAvgTemp() {
	global $arrTemps;
	echo number_format((array_sum($arrTemps)/count($arrTemps)),2);
}

//	POST SWITCH FOR DEVICE CONTROL
switch (true) {
	case isset($_POST['Heat_1_On']):
		exec('sudo /home/pi/cooler/run -h1 -r',$retval);
		break;
	case isset($_POST['Heat_1_Off']):
		exec('sudo /home/pi/cooler/run -h1 -s',$retval);
		break;
	case isset($_POST['Heat_2_On']):
		exec('sudo /home/pi/cooler/run -h2 -r',$retval);
		break;
	case isset($_POST['Heat_2_Off']):
		exec('sudo /home/pi/cooler/run -h2 -s',$retval);
		break;
	case isset($_POST['Heat_3_On']):
		exec('sudo /home/pi/cooler/run -h3 -r',$retval);
		break;
	case isset($_POST['Heat_3_Off']):
		exec('sudo /home/pi/cooler/run -h3 -s',$retval);
		break;
	case isset($_POST['Heat_4_On']):
		exec('sudo /home/pi/cooler/run -h4 -r',$retval);
		break;
	case isset($_POST['Heat_4_Off']):
		exec('sudo /home/pi/cooler/run -h4 -s',$retval);
		break;
	case isset($_POST['Fan_1_On']):
		exec('sudo /home/pi/cooler/run -f1 -r',$retval);
		break;
	case isset($_POST['Fan_1_Off']):
		exec('sudo /home/pi/cooler/run -f1 -s',$retval);
		break;
	case isset($_POST['All_On']):
		exec('sudo /home/pi/cooler/run -a',$retval);
		break;
	case isset($_POST['All_Off']):
		exec('sudo /home/pi/cooler/run -x',$retval);
		break;
	case isset($_POST['temp_set_submit']):
		setTemperatureSetting($_POST['temp_set_value']);
	default:
		//	Nothing was correct
		break;
}

?>