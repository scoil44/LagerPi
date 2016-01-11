<?php
	include 'includes/master.php';
	getHead();
	getNav();
?>
<div class="container" id="alerts">
	<div class="row">
		<div class="col-xs-12 text-center"><h3>Alerts:</h3></div>
		<div class="col-xs-12 text-center">
			<div class="alert alert-success">
				<strong>Relax!</strong> Nothing to report.
			</div>
		</div>
	</div>
</div>

<div class="container" id="beers">
	<div class="row">
		<div class="col-xs-12 text-center"><h3>Beers:</h3></div>
	</div>
	<div class="row">
		<div id="beer1" class="quickGraph col-xs-12 col-sm-4 bg-primary">
			<script type = "text/javascript" >
			var data = <?php getBeerTemperatures(99)?>;
			var data2 = <?php getBeerTemperatures(97)?>;
			window.onload = graph(beer1,data,data2);
			</script>
		</div>
		<div id="beer2" class="quickGraph col-xs-12 col-sm-4 bg-danger">
			<p>This would be the second beer.</p>
			<p>This functionality is not yet implemented.</p>
		</div>
		<div id="beer3" class="quickGraph col-xs-12 col-sm-4 bg-info">
			<p>This would be the third beer</p>
			<p>This functionality is not yet implemented.</p>
		</div>
	</div>
</div>

<?php
	getFooter();
?>