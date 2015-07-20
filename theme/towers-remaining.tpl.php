<p>The following towers were left standing during this game:</p>
<ul>
	<?php foreach ($stats['buildings'] as $building_name => $bool): ?>
	  <?php if ($bool): ?>
		<li> <?php print $building_name; ?> </li>
	  <?php endif; ?>
	<?php endforeach; ?>
</ul>