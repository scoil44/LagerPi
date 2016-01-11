<?php
	include 'includes/master.php';
	getHead();
	getNav();
?>

<div class="container-fluid">
	<div class="row">
		<div class="col-xs-12 col-sm-9 col-lg-5 col-lg-offset-2">
			<div class="row">
				<div id="temperatureGraph" style="max-width:720px;min-height:220px;margin:0 auto;width:100%">
					<script type = "text/javascript">
					var data = <?php getBeerTemperatures(0)?>;
					var data2 = <?php getBeerTemperatures(1)?>;
					var data3 = <?php getBeerTemperatures(2)?>;
					window.onload = graph(temperatureGraph,data,data2,data3);
					</script>
				</div>
			</div>
			<div class="row text-center" style="max-width:720px;">
				<div class="col-xs-12 col-sm-4">
					<p>Avg. Temp: <code><?php getAvgTemp();?>&deg; F</code></p>
				</div>
				<div class="col-xs-6 col-sm-4">
					<p>Time Lagering: <code>1 kSec</code></p>
				</div>
				<div class="col-xs-6 col-sm-4">
					<p>Time Left: <code>16 &#8532; min</code></p>
				</div>
			</div>
		</div>

		<div class="col-xs-12 col-sm-3 col-lg-offset-2">
			<div class="row">
				<table class="table table-striped">
					<tr>
						<th>Name:</th>
						<td>Never Forget</td>
					</tr>
					<tr>
						<th>Style:</th>
						<td>Murican Pale Lager</td>
					</tr>
					<tr>
						<th>Start Lagering:</th>
						<td>March 30, 2013</td>
					</tr>
					<tr>
						<th>Finish Lagering:</th>
						<td>May 30, 2013</td>
					</tr>
				</table>
			</div>
			<div class="row text-center">
				<button>
					Edit Beer Parameters
				</button>
			</div>
		</div>
	</div>
</div>


<?php
	getFooter();
?>