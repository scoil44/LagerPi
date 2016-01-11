<?php
	include 'includes/master.php';
	getHead();
	getNav();
?>
<div class="container">
	<div class="row">
		<div class="col-xs-12 col-sm-4">
			<table class="table table-striped">
				<tr>
					<th>Device</th>
					<th>ID</th>
					<th>Status</th>
				</tr>
				<?php getConfig(); ?>
			</table>
		</div>
		<div class="col-xs-12 col-sm-4">
			<table class="table table-striped">
				<tr><th>Device Overides</th></tr>
				<tr><td><form method="POST" action="">
					<span>Heater 1</span>
					<span style="float:right;">
						<input type="submit" name="Heat_1_On"  value="On">
						<input type="submit" name="Heat_1_Off"  value="Off">
					</span></form></td></tr>
				<tr><td><form method="POST" action="">
					<span>Heater 2</span>
					<span style="float:right;">
						<input type="submit" name="Heat_2_On"  value="On">
						<input type="submit" name="Heat_2_Off"  value="Off">
					</span></form></td></tr>
				<tr><td><form method="POST" action="">
					<span>Heater 3</span>
					<span style="float:right;">
						<input type="submit" name="Heat_3_On"  value="On">
						<input type="submit" name="Heat_3_Off"  value="Off">
					</span></form></td></tr>
				<tr><td><form method="POST" action="">
					<span>Heater 4</span>
					<span style="float:right;">
						<input type="submit" name="Heat_4_On"  value="On">
						<input type="submit" name="Heat_4_Off"  value="Off">
					</span></form></td></tr>
				<tr><td><form method="POST" action="">
					<span>Fans</span>
					<span style="float:right;">
						<input type="submit" name="Fan_1_On"  value="On">
						<input type="submit" name="Fan_1_Off"  value="Off">
					</span></form></td></tr>
				<tr><td><form method="POST" action="">
					<span>All Devices</span>
					<span style="float:right;">
						<input type="submit" name="All_On"  value="On">
						<input type="submit" name="All_Off"  value="Off">
					</span></form></td></tr>
			</table>
		</div>
		<div class="col-xs-12 col-sm-4">
			<table class="table table-striped">
				<tr><th>Desired Temperatures</th></tr>
				<tr><td>
					<span>Currenlty:</span>
					<span style="float:right"><?php getTemperatureSetting(); ?> &deg;C</span>
				</td></tr>
				<tr><td><form method="POST" action=""><span style="float:right;">
					<input type="number" max="30" min="0" step="0.01" name="temp_set_value" required>
					<input type="submit" name="temp_set_submit" value="Submit"></span>
				</form></td></tr>
			</table>
		</div>
	</div>
</div>

<?php
	getFooter();
?>