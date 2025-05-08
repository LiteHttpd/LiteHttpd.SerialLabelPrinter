extern const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Service Started</title>
	<style>
		body {
			font-family: sans-serif;
			text-align: center;
			background-color: #f5f5f5;
			padding-top: 100px;
		}
		h1 {
			color: #4CAF50;
		}
		p {
			font-size: 18px;
			color: #333;
		}
	</style>
</head>
<body>
	<h1>✅ Serial Label Printer Service Started</h1>
	<p>The service is running. You can now start printing labels.</p>
</body>
</html>
)rawliteral";

extern const char* html418 = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>418 - I'm a teapot</title>
    <style>
        body {
            background-color: #fffbe6;
            color: #333;
            font-family: 'Comic Sans MS', cursive, sans-serif;
            text-align: center;
            padding-top: 100px;
        }
        h1 {
            font-size: 48px;
            color: #a0522d;
        }
        p {
            font-size: 24px;
        }
        .teapot {
            font-size: 80px;
            margin: 30px 0;
        }
        .steam {
            animation: steam 2s infinite;
        }
        button {
            padding: 10px 20px;
            font-size: 18px;
            background-color: #a0522d;
            color: white;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            margin-top: 20px;
        }
        button:hover {
            background-color: #8b4513;
        }
        @keyframes steam {
            0% { opacity: 0; transform: translateY(0); }
            50% { opacity: 1; transform: translateY(-10px); }
            100% { opacity: 0; transform: translateY(-20px); }
        }
    </style>
</head>
<body>
    <h1>418 - I'm a teapot</h1>
    <div class="teapot">🫖</div>
    <p class="steam">Sorry, I can't brew coffee. I'm a teapot!</p>
    <button onclick="brewCoffee()">Try to brew coffee</button>
    <p><em>(Yes, this is a real HTTP status code.)</em></p>

    <script>
        function brewCoffee() {
            alert("🛑 ERROR 418: Cannot brew coffee. This unit is a teapot.");
        }
    </script>
</body>
</html>
)rawliteral";
