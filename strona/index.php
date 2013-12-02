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

$measure= "SELECT * FROM weather"; // Zapytanie do bazy danych

$result = mysql_query($measure); // wykonanie zapytania oraz zwrocenie wynikow
$num_data = mysql_numrows($result); // Sprawdzenie ilosci wierszy

echo "Stacja pogody - pomiary\n";
if($num_data > 0)
{
       echo "\t<table border = 1>\n";
       echo "\t\t<tr>\n";
       echo "\t\t\t<td>Data</td>\n";
       echo "\t\t\t<td>Godzina</td>\n";
       echo "\t\t\t<td>Temperatura</td>\n";
       echo "\t\t\t<td>Wilgotność</td>\n";
       echo "\t\t\t<td>Ciśnienie</td>\n";
       echo "\t\t</tr>\n";
}

for ($i = $num_data - 1; $i >= 0; $i--)
{
       $date = mysql_result($result, $i, "date");
       $time = mysql_result($result, $i, "time");
       $temperature = mysql_result($result, $i, "temperature");
       $humidity = mysql_result($result, $i, "humidity");
       $pressure = mysql_result($result, $i, "pressure");

       echo "\t\t<tr>\n";
       echo "\t\t\t<td>$date</td>\n";
       echo "\t\t\t<td>$time</td>\n";
       echo "\t\t\t<td>$temperature &deg;C</td>\n";
       echo "\t\t\t<td>$humidity%</td>\n";
       echo "\t\t\t<td>$pressure hPa</td>\n";
       echo "\t\t</tr>\n";
}

if($num_data > 0) echo "\t</table>\n";

mysql_close();
?>
</body>
</html>
