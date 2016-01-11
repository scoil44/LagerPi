<?php
	function getNav(){
?>

<body>
<nav class="navbar navbar-inverse">

<header class="container-fluid">
	<div class="row">
		<div class="col-xs-6 bg-success">
			<h1>CoolRod</h1>
		</div>
		<div class="col-xs-6 bg-warning text-right">
			<h1><small>a <abbr title="Coi-Berry-Pi">CBPI</abbr> Project</small></h1>
		</div>
	</div>
</header>
<div class="container-fluid">
	<a class="navbar-brand pull-right" href="config.php">Config</a>
	<div class="navbar-header">
		<a class="navbar-brand" href="index.php">Home</a>
		<button type="button" class="navbar-toggle navbar-btn" data-toggle="collapse" data-target="#navFull">
			<span class="icon-bar"></span>
			<span class="icon-bar"></span>
			<span class="icon-bar"></span>
		</button>
	</div>
	<div class="collapse navbar-collapse" id="navFull">
		<ul class="nav navbar-nav">
			<li><a href="monitor.php">Monitor</a></li>
			<li><a href="commands.php">Command Log</a></li>
			<li><a href="status.php">Status</a></li>
		</ul>
	</div>
</div>

</nav>

<?php
	}
?>