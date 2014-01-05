<html>
<head>
<title>Stacja pogodowa - projekt inżynierski</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<body>
<?
$user = 'makam';
$password = '28CkcBUj';
$database = 'makam';
$address = 'mysql.agh.edu.pl';

 mysql_connect($address, $user, $password);
@mysql_select_db($database) or die("Nie udało się wybrać bazy danych");
echo "Stacja pogody - pomiary\n";

$measure = "SELECT * FROM weather"; // Sprawdzenie calkowitej ilosci wierszy
$result = mysql_query($measure); // wykonanie zapytania oraz zwrocenie wynikow
$total_data = mysql_numrows($result); // Sprawdzenie ilosci wierszy
$rstart_date = explode("-", mysql_result($result, 0, "date"));
$rstart_time = explode(":", mysql_result($result, 0, "time"));
$rend_date = explode("-", mysql_result($result, $total_data - 1, "date"));
$rend_time = explode(":", mysql_result($result, $total_data - 1, "time"));

if(isset($_POST["delay"]))
{
	$delay = $_POST["delay"];
	$change_delay = "UPDATE  `makam`.`weather_config` SET  `delay` =  '$delay' WHERE  `weather_config`.`index` =0";
	$result = mysql_query($change_delay);
	header('Location: index.php');
}
if((isset($_GET["start_hour"])) and
	(isset($_GET["start_minute"])) and
	(isset($_GET["start_second"])) and
	(isset($_GET["start_day"])) and
	(isset($_GET["start_month"])) and
	(isset($_GET["start_year"])) and
	(isset($_GET["end_hour"])) and
	(isset($_GET["end_minute"])) and
	(isset($_GET["end_second"])) and
	(isset($_GET["end_day"])) and
	(isset($_GET["end_month"])) and
	(isset($_GET["end_year"]))	
	)
{
	$start_date_str = $_GET["start_year"]."-".$_GET["start_month"]."-".$_GET["start_day"]." ".$_GET["start_hour"].":".$_GET["start_minute"].":".$_GET["start_second"];
	$start_date = new DateTime($start_date_str);
	$end_date_str = $_GET["end_year"]."-".$_GET["end_month"]."-".$_GET["end_day"]." ".$_GET["end_hour"].":".$_GET["end_minute"].":".$_GET["end_second"];
	$end_date = new DateTime($end_date_st);
	if($end_date >= $start_date)
	{
		$measure= "SELECT * FROM weather WHERE CONCAT(`date`,' ',`time`) >= '$start_date_str' and CONCAT(`date`,' ',`time`) <= '$end_date_str'"; // Zapytanie do bazy danych
	}
	else
	{
		die("Niepoprawna data");
	}
}
else
{
	$measure= "SELECT * FROM weather"; // Zapytanie do bazy danych
}
$config = "SELECT delay from weather_config";

$result = mysql_query($config);
$delay = mysql_result($result, 0);

$result = mysql_query($measure); // wykonanie zapytania oraz zwrocenie wynikow
$num_data = mysql_numrows($result); // Sprawdzenie ilosci wierszy

echo "<form action=\"index.php\" method=\"get\">\n";
echo "Początkowa data<br>\n";
echo "Godzina: <input name=\"start_hour\" size=\"2\" maxlength=\"2\" value=\"$rstart_time[0]\"/>\n";
echo "Minuta: <input name=\"start_minute\" size=\"2\" maxlength=\"2\" value=\"$rstart_time[1]\"/>\n";
echo "Sekunda: <input name=\"start_second\" size=\"2\" maxlength=\"2\" value=\"$rstart_time[2]\"/>\n";
echo "Dzień: <input name=\"start_day\" size=\"2\" maxlength=\"2\" value=\"$rstart_date[2]\"/>\n";
echo "Miesiąc: <input name=\"start_month\" size=\"2\" maxlength=\"2\" value=\"$rstart_date[1]\"/>\n";
echo "Rok: <input name=\"start_year\" size=\"4\" maxlength=\"4\" value=\"$rstart_date[0]\"/>\n<br>\n";
echo "Końcowa data<br>\n";
echo "Godzina: <input name=\"end_hour\" size=\"2\" maxlength=\"2\" value=\"$rend_time[0]\"/>\n";
echo "Minuta: <input name=\"end_minute\" size=\"2\" maxlength=\"2\" value=\"$rend_time[1]\"/>\n";
echo "Sekunda: <input name=\"end_second\" size=\"2\" maxlength=\"2\" value=\"$rend_time[2]\"/>\n";
echo "Dzień: <input name=\"end_day\" size=\"2\" maxlength=\"2\" value=\"$rend_date[2]\"/>\n";
echo "Miesiąc: <input name=\"end_month\" size=\"2\" maxlength=\"2\" value=\"$rend_date[1]\"/>\n";
echo "Rok: <input name=\"end_year\" size=\"4\" maxlength=\"4\" value=\"$rend_date[0]\"/>\n";
echo "<br>\n<input type=\"submit\" value=\"Zaaktualizuj dane\" />\n";
echo "</form>\n";

echo "<form action=\"index.php\" method=\"post\">\n";
echo "\tOpoznienie: <select name=\"delay\" value=$delay>\n";
for ($i = 0; $i <= 600; $i++)
{
	if($i != $delay) echo "<option>$i</option>";
	else echo "<option selected=\"selected\">$i</option>";
	if($i % 3 == 0) echo "\n";
}
echo "\n\t</select>\n";
echo "\t<input type=\"submit\" value=\"Zaaktualizuj opóźnienie\" />\n";
echo "</form>\n";

echo "Znaleziono $num_data/$total_data wyników";

if($num_data > 0)
{
       echo "\t<table border = 1>\n";
       echo "\t\t<tr align = center>\n";
       echo "\t\t\t<td rowspan = 2>Data</td>\n";
       echo "\t\t\t<td rowspan = 2>Godzina</td>\n";
       echo "\t\t\t<td colspan = 2>DHT-22</td>\n";
       echo "\t\t\t<td colspan = 2>BMP085</td>\n";
       echo "\t\t</tr>\n";
       echo "\t\t<tr align = center>\n";
       echo "\t\t\t<td>Temperatura</td>\n";
       echo "\t\t\t<td>Wilgotność</td>\n";
       echo "\t\t\t<td>Temperatura</td>\n";
       echo "\t\t\t<td>Ciśnienie</td>\n";
       echo "\t\t</tr>\n";
}

for ($i = $num_data - 1; $i >= 0; $i--)
{
       $date = mysql_result($result, $i, "date");
       $time = mysql_result($result, $i, "time");
       $dht22_temperature = mysql_result($result, $i, "dht22_temperature");
       $dht22_humidity = mysql_result($result, $i, "dht22_humidity");
       $bmp085_temperature = mysql_result($result, $i, "bmp085_temperature");
       $bmp085_pressure = mysql_result($result, $i, "bmp085_pressure");

       echo "\t\t<tr align = center>\n";
       echo "\t\t\t<td>$date</td>\n";
       echo "\t\t\t<td>$time</td>\n";
       echo "\t\t\t<td>$dht22_temperature &deg;C</td>\n";
       echo "\t\t\t<td>$dht22_humidity%</td>\n";
       echo "\t\t\t<td>$bmp085_temperature &deg;C</td>\n";
       echo "\t\t\t<td>$bmp085_pressure hPa</td>\n";
       echo "\t\t</tr>\n";
}

if($num_data > 0) echo "\t</table>\n";

mysql_close();
?>
</body>
</html>
