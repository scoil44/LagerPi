<?php
	include 'includes/master.php';
	getHead();
	getNav();
?>
<div class="container">
	<div class="row">
		<table class="table table-striped">
			<tr>
				<th>Time</th>
				<th>Action</th>
				<th>Result</th>
				<th>Source</th>
			</tr>
			<?php getCommands(); ?>
		</table>
	</div>
</div>

<?php
	getFooter();
?>