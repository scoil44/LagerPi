<?php
	include 'includes/master.php';
	getHead();
	getNav();
?>
<div class="container">
	<div class="row">
		<table class="table table-striped">
			<tr>
				<th>GPIO</th>
				<th>Device</th>
				<th>ID</th>
				<th>Status</th>
			</tr>
			<?php getDeviceStatus(); ?>
		</table>
	</div>
</div>

<?php
	getFooter();
?>