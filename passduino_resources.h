const char PAGE_HTML[] = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <title>Passduino</title>

    <!-- Latest compiled and minified CSS -->
    <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
    <!-- Optional theme -->
    <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css" integrity="sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp" crossorigin="anonymous">

  </head>
  <body>
    <div class="container">
      <div class="row">
        Click button to activate macro:
      </div>
      <div class="row">
%%macros%%
      </div>
    </div>

    <!-- Latest compiled and minified jQuery -->
    <script src="//cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>

    <script>
function trigger(macro) {
  var xmlHttp = new XMLHttpRequest();
  xmlHttp.onreadystatechange = function() { return; };
  xmlHttp.open("GET", "/macro?" + macro, true); // true for asynchronous
  xmlHttp.send(null);
}
    </script>

    <!-- Latest compiled and minified JavaScript -->
    <script src="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script>

  </body>
</html>
)=====";

const char PAGE_HTML_MACRO_ROW[] = R"=====(
        <div class="col-xs-6 col-sm-4" style="margin-top: 0.5em; margin-bottom: 0.5em;">
          <button type="button" class="btn btn-primary btn-block" onclick="trigger('%%name%%');">%%name%%</button>
        </div>
)=====";
