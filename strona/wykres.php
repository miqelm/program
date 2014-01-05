<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<title>Wykresy</title>
	<link href="flot/examples/examples.css" rel="stylesheet" type="text/css">
	<!--[if lte IE 8]><script language="javascript" type="text/javascript" src="flot/excanvas.min.js"></script><![endif]-->
	<script language="javascript" type="text/javascript" src="flot/jquery.js"></script>
	<script language="javascript" type="text/javascript" src="flot/jquery.flot.js"></script>
	<script language="javascript" type="text/javascript" src="flot/jquery.flot.time.js"></script>
	<script type="text/javascript">

	$(function() {
	
<?
$user = 'makam';
$password = '28CkcBUj';
$database = 'makam';
$address = 'mysql.agh.edu.pl';
 mysql_connect($address, $user, $password);
@mysql_select_db($database) or die("Nie udało się wybrać bazy danych");
$measure= "SELECT * FROM weather WHERE CONCAT(`date`,' ',`time`) >= '2014-01-04 01:22:31' and CONCAT(`date`,' ',`time`) < '2014-01-04 15:50:22'"; // Zapytanie do bazy danych
$result = mysql_query($measure); // wykonanie zapytania oraz zwrocenie wynikow
$num_data = mysql_numrows($result); // Sprawdzenie ilosci wierszy
if($num_data > 0)
{
	echo "		var dht22_temperatures = [";
	for ($i = 0; $i < $num_data; $i++)
	{
		$date = mysql_result($result, $i, "date");
		$time = mysql_result($result, $i, "time");
		$dht22_temperature = mysql_result($result, $i, "dht22_temperature");
		$dht22_humidity = mysql_result($result, $i, "dht22_humidity");
		$bmp085_temperature = mysql_result($result, $i, "bmp085_temperature");
		$bmp085_pressure = mysql_result($result, $i, "bmp085_pressure");

		$test = new DateTime($date." ".$time);
		if($i != $num_data - 1)
		{
			echo ("[".$test->getTimestamp()*1000).",".$dht22_temperature."], ";
		}
		else
		{
			echo ("[".$test->getTimestamp()*1000).",".$dht22_temperature."]];\n\n";
		}
	}
	echo "		var bmp085_temperatures = [";
	for ($i = 0; $i < $num_data; $i++)
	{
		$date = mysql_result($result, $i, "date");
		$time = mysql_result($result, $i, "time");
		$dht22_temperature = mysql_result($result, $i, "dht22_temperature");
		$dht22_humidity = mysql_result($result, $i, "dht22_humidity");
		$bmp085_temperature = mysql_result($result, $i, "bmp085_temperature");
		$bmp085_pressure = mysql_result($result, $i, "bmp085_pressure");

		$test = new DateTime($date." ".$time);
		if($i != $num_data - 1)
		{
			echo ("[".$test->getTimestamp()*1000).",".$bmp085_temperature."], ";
		}
		else
		{
			echo ("[".$test->getTimestamp()*1000).",".$bmp085_temperature."]];\n\n";
		}
	}
	echo "		var bmp085_pressures = [";
	for ($i = 0; $i < $num_data; $i++)
	{
		$date = mysql_result($result, $i, "date");
		$time = mysql_result($result, $i, "time");
		$dht22_temperature = mysql_result($result, $i, "dht22_temperature");
		$dht22_humidity = mysql_result($result, $i, "dht22_humidity");
		$bmp085_temperature = mysql_result($result, $i, "bmp085_temperature");
		$bmp085_pressure = mysql_result($result, $i, "bmp085_pressure");

		$test = new DateTime($date." ".$time);
		if($i != $num_data - 1)
		{
			echo ("[".$test->getTimestamp()*1000).",".$bmp085_pressure."], ";
		}
		else
		{
			echo ("[".$test->getTimestamp()*1000).",".$bmp085_pressure."]];\n\n";
		}
	}
	echo "		var dht22_humidities = [";
	for ($i = 0; $i < $num_data; $i++)
	{
		$date = mysql_result($result, $i, "date");
		$time = mysql_result($result, $i, "time");
		$dht22_temperature = mysql_result($result, $i, "dht22_temperature");
		$dht22_humidity = mysql_result($result, $i, "dht22_humidity");
		$bmp085_temperature = mysql_result($result, $i, "bmp085_temperature");
		$bmp085_pressure = mysql_result($result, $i, "bmp085_pressure");

		$test = new DateTime($date." ".$time);
		if($i != $num_data - 1)
		{
			echo ("[".$test->getTimestamp()*1000).",".$dht22_humidity."], ";
		}
		else
		{
			echo ("[".$test->getTimestamp()*1000).",".$dht22_humidity."]];\n\n";
		}
	}
}

mysql_close();
?>
		function doPlot() {
			$.plot("#temperatury", [
				{ data: dht22_temperatures, label: "Temperatura DHT-22 (&deg;C)" },
				{ data: bmp085_temperatures, label: "Temperatura BMP085 (&deg;C)" }
			], {
				xaxes: [ { mode: "time" ,
						   timeformat: "%h:%M %d.%m.%Y" }],
				yaxes: [ { min: 0,
					tickFormatter: function(val, axis) { return val < axis.max ? val.toFixed(2) : val.toFixed(2) + " &deg;C";}
				 }, {
				} ],
				legend: { position: "nw" }
			});
			
			$.plot("#reszta", [
				{ data: dht22_humidities, label: "Wilgotność DHT-22 (%)" },
				{ data: bmp085_pressures, label: "Ciśnienie BMP085 (hPa)", yaxis: 2 }
			], {
				xaxes: [ { mode: "time" ,
						   timeformat: "%h:%M\n%d.%m.%Y" }],
				yaxes: [ { min: 0,
					tickFormatter: function(val, axis) { return val < axis.max ? val.toFixed(2) : val.toFixed(0) + " %";}
				 }, {
					// align if we are to the right
					alignTicksWithAxis: 1,
					position: "right",
					tickFormatter: function(val, axis) { return val < axis.max ? val.toFixed(2) : val.toFixed(2) + " hPa";}
				} ],
				legend: { position: "nw" }
			});
		}

		doPlot();
	});

	</script>
</head>
<body>

	<div id="content">

		<div class="demo-container">
			<div id="temperatury" class="demo-placeholder"></div>
		</div>
		
		<div class="demo-container">
			<div id="reszta" class="demo-placeholder"></div>
		</div>

	</div>

</body>
</html>
