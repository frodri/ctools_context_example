<?php foreach ($stats['players'] as $player): ?>
  <p>A Level <?php print $player['level']; ?> <?php print $player['hero']; ?> 
  had <?php print $player['kills']; ?> kills, <?php print $player['deaths']; ?> deaths,
   <?php print $player['assists']; ?> assists and a creep score 
   of <?php print $player['last_hits']; ?> last hits and <?php print $player['denies']; ?> denies.</p>
<?php endforeach; ?>