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

if(isset($_POST["delay"]))
{
	$delay = $_POST["delay"];
	$change_delay = "UPDATE  `makam`.`weather_config` SET  `delay` =  '$delay' WHERE  `weather_config`.`index` =0";
	$result = mysql_query($change_delay);
	header('Location: index.php');
}

$measure= "SELECT * FROM weather"; // Zapytanie do bazy danych
$config = "SELECT delay from weather_config";

$result = mysql_query($config);
$delay = mysql_result($result, 0);

echo "<form action=\"index.php\" method=\"get\">\n";
echo "Początkowa data<br>\n";
echo "Godzina: <input name=\"start_hour\" size=\"2\" maxlength=\"2\" />\n";
echo "Minuta: <input name=\"start_minute\" size=\"2\" maxlength=\"2\" />\n";
echo "Dzień: <input name=\"start_day\" size=\"2\" maxlength=\"2\" />\n";
echo "Miesiąc: <input name=\"start_month\" size=\"2\" maxlength=\"2\" />\n";
echo "Rok: <input name=\"start_year\" size=\"4\" maxlength=\"4\" />\n<br>\n";
echo "Końcowa data<br>\n";
echo "Godzina: <input name=\"end_hour\" size=\"2\" maxlength=\"2\" />\n";
echo "Minuta: <input name=\"end_minute\" size=\"2\" maxlength=\"2\" />\n";
echo "Dzień: <input name=\"end_day\" size=\"2\" maxlength=\"2\" />\n";
echo "Miesiąc: <input name=\"end_month\" size=\"2\" maxlength=\"2\" />\n";
echo "Rok: <input name=\"end_year\" size=\"4\" maxlength=\"4\" />\n";
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

$result = mysql_query($measure); // wykonanie zapytania oraz zwrocenie wynikow
$num_data = mysql_numrows($result); // Sprawdzenie ilosci wierszy

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
