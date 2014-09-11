/**
	Flooded Veins
	Dynamic map with a cave leading to gems filled with water deep down.
	
	@author Sven2, Maikel
*/

#include Library_Map


// Scenario properties which can be set later by the lobby options.
static const SCENOPT_MapSize = 1;

// Called be the engine: draw the complete map here.
protected func InitializeMap(proplist map)
{
	// Retrieve the settings according to the MapSize setting.
	var map_size;
	if (SCENOPT_MapSize == 1)
		map_size = [100, 150]; 
	if (SCENOPT_MapSize == 2)
		map_size = [100, 175];
	if (SCENOPT_MapSize == 3)
		map_size = [100, 200];
	
	// Set the map size.
	map->Resize(map_size[0], map_size[1]);
	
	// The map is completely covered in earth.
	map->Draw("Earth");
	
	// Draw a cavern somewhere at the top where the settlement is located.
	// This takes up the top 25 landscape pixels.
	DrawCavern(map);
	
	// Draw the middle section of the map with useful resources.
	// This takes 50, 55 or 60 landscape pixels.
	DrawMiddle(map, 45 + 5 * SCENOPT_MapSize);
	
	// Draw the gem veins including it being flooded at the bottom of the map.
	// This takes up the remaining landscape pixels, which depends on the map size.
    DrawGemVeins(map, 55 + 20 * SCENOPT_MapSize);
	
	// Return true to tell the engine a map has been successfully created.
	return true;
}

// Draws the top cavern where the player starts it settlement.
public func DrawCavern(proplist map)
{
    var wdt = map.Wdt;
    var hgt = map.Hgt;
    var cavern_hgt = 20;
    
    // Create a mask for the cavern part.
    var map_top = {Algo = MAPALGO_Rect, X = 0, Y = 0, Wdt = wdt, Hgt = cavern_hgt};
    
    // The top part is mainly granite and rock.
    map->DrawMaterial("Earth-earth_rough", map_top, 2, 16);
	map->DrawMaterial("Earth-earth_dry", map_top, 2, 16);
	map->DrawMaterial("Earth-earth_midsoil", map_top, 4, 12);
	map->DrawMaterial("Granite", map_top, 3, 60);
	map->DrawMaterial("Tunnel", map_top, 2, 16);
	map->DrawMaterial("Rock-rock_cracked", map_top, 3, 20);
	map->DrawMaterial("Rock", map_top, 3, 20);
   
    // The cavern is located a bit to the left and can be entered from the left side of the map.
    var cavern = {Algo = MAPALGO_Ellipsis, X = wdt / 2 - 10, Y = cavern_hgt - 7, Wdt = 20, Hgt = 7};
    cavern = {Algo = MAPALGO_Or, Op = [cavern, {Algo = MAPALGO_Turbulence, Iterations = 4, Seed = Random(65536), Op = cavern}]};
    cavern = {Algo = MAPALGO_Or, Op = [cavern, {Algo = MAPALGO_Rect, X = wdt / 2 - 30, Y = cavern_hgt - 7, Wdt = 40, Hgt = 7}]};
    cavern = {Algo = MAPALGO_And, Op = [cavern, map_top]};
    map->Draw("Tunnel", cavern);
    map->DrawMaterial("Tunnel-brickback", cavern, 5, 32);
    map->DrawMaterial("Sky", cavern, 5, 16);
    map->DrawMaterial("Sky", cavern, 4, 16);
    map->DrawMaterial("Sky", cavern, 3, 16);
    var cavern_ground = {Algo = MAPALGO_Border, Bottom = -2, Op = cavern};
    map->DrawMaterial("Brick", cavern_ground, 3, 80);

    // Draw the left tunnel.
    var tunnel_left = {Algo = MAPALGO_Polygon, X = [0, wdt / 2 - 30], Y = [cavern_hgt, cavern_hgt], Wdt = -5, Open = 1, Empty = 1};
    tunnel_left = {Algo = MAPALGO_Or, Op = [tunnel_left, {Algo = MAPALGO_Turbulence, Amplitude = 6, Scale = 8, Iterations = 4, Seed = Random(65536), Op = tunnel_left}]};
    tunnel_left = {Algo = MAPALGO_And, Op = [tunnel_left, map_top]};
    map->Draw("Tunnel", tunnel_left);
    map->DrawMaterial("Tunnel-brickback", tunnel_left, 3, 32);
    map->DrawMaterial("Sky", tunnel_left, 5, 16);
    map->DrawMaterial("Sky", tunnel_left, 3, 24);
    var tunnel_entrance = {Algo = MAPALGO_And, Op = [tunnel_left, {Algo = MAPALGO_Rect, X = 0, Y = 0, Wdt = 2, Hgt = cavern_hgt}]};
    map->Draw("Sky", tunnel_entrance);
    var tunnel_left_ground = {Algo = MAPALGO_Border, Bottom = -2, Op = tunnel_left};
    tunnel_left_ground = {Algo = MAPALGO_Or, Op = [tunnel_left, {Algo = MAPALGO_Turbulence, Amplitude = 6, Scale = 6, Iterations = 2, Seed = Random(65536), Op = tunnel_left_ground}]};
    tunnel_left_ground = {Algo = MAPALGO_And, Op = [tunnel_left_ground, {Algo = MAPALGO_Not, Op = map_top}]};
    map->DrawMaterial("Brick", tunnel_left_ground, 3, 24);

    // Draw the right tunnel.
    var tunnel_right = {Algo = MAPALGO_Polygon, X = [wdt / 2 + 10, wdt / 2 + 30, wdt / 2 + 36], Y = [cavern_hgt - 4, cavern_hgt - 8, cavern_hgt + 6], Wdt = -4, Open = 1, Empty = 1};
    tunnel_right = {Algo = MAPALGO_Or, Op = [tunnel_right, {Algo = MAPALGO_Turbulence, Amplitude = 8, Scale = 6, Iterations = 4, Seed = Random(65536), Op = tunnel_right}]};
    //tunnel_right = {Algo = MAPALGO_And, Op = [tunnel_right, map_top]};
    map->Draw("Tunnel", tunnel_right);
    map->DrawMaterial("Tunnel-brickback", tunnel_right, 3, 24);
    return;
}

// Draws the middle section of the map with useful resources.
public func DrawMiddle(proplist map, int size)
{
    var wdt = map.Wdt;
    var hgt = map.Hgt;

    // Create a mask for the middle part.
    var map_middle = {Algo = MAPALGO_Rect, X = 0, Y = 25, Wdt = wdt, Hgt = size};
    
    // Fill the middle part with resources.
    map->DrawMaterial("Earth-earth_rough", map_middle, 2, 16);
	map->DrawMaterial("Earth-earth_dry", map_middle, 2, 16);
	map->DrawMaterial("Earth-earth_midsoil", map_middle, 4, 12);
	map->DrawMaterial("Granite", map_middle, 3, 10);
	map->DrawMaterial("Tunnel", map_middle, 2, 8);
	map->DrawMaterial("Rock-rock_cracked", map_middle, 3, 8);
	map->DrawMaterial("Rock", map_middle, 3, 8);
   	map->DrawMaterial("Ore", map_middle, 6, 16);
   	map->DrawMaterial("Firestone", map_middle, 5, 12);
   	map->DrawMaterial("Coal", map_middle, 6, 16);
    
	// Create a tunnel system which covers the middle part.
	var tunnel = {Algo = MAPALGO_Rect, X = 0, Y = 0, Wdt = 0, Hgt = 0};
	for (var i = 0; i < 5; i++)
	{
		var x1 = RandomX(8, wdt - 8);
		var y1 = RandomX(30, 10 + size);
		var dir = RandomX(-15, 15) + Random(2) * 180;
		var dist = RandomX(20, 28);
		var x2 = BoundBy(x1 + Cos(dir, dist), 4, wdt - 4);
		var y2 = BoundBy(y1 + Sin(dir, dist), 30, 15 + size);
		var add_tunnel = {Algo = MAPALGO_Polygon, X = [x1, x2], Y = [y1, y2], Wdt = -RandomX(3, 4), Open = 1, Empty = 1};
		var tunnel = {Algo = MAPALGO_Or, Op = [tunnel, add_tunnel]};
	}
	tunnel = {Algo = MAPALGO_Turbulence, Amplitude = 6, Scale = 8, Iterations = 4, Seed = Random(65536), Op = tunnel};
	map->Draw("Tunnel", tunnel);
	var tunnel_ground = {Algo = MAPALGO_Border, Bottom = -2, Op = tunnel};
	map->Draw("Earth", tunnel_ground);
	map->DrawMaterial("Earth-earth_rough", tunnel_ground, 2, 16);
	map->DrawMaterial("Earth-earth_dry", tunnel_ground, 2, 16);
	map->DrawMaterial("Earth-earth_midsoil", tunnel_ground, 4, 12);
	return;
}

// Draws the gem veins including it being flooded at the bottom of the map.
public func DrawGemVeins(proplist map, int size)
{
    var wdt = map.Wdt;
    var hgt = map.Hgt;
    
    // Create a mask for the bottom part.
    var map_bottom = {Algo = MAPALGO_Rect, X = 0, Y = hgt - size, Wdt = wdt, Hgt = size};
    
    // Fill the bottom with mostly granite materials.
    map->Draw("Granite", map_bottom);
    map->DrawMaterial("Firestone", map_bottom, 2, 6);
	map->DrawMaterial("Rock-rock_cracked", map_bottom, 3, 12);
	map->DrawMaterial("Rock", map_bottom, 3, 12);
	map->DrawMaterial("Earth-earth_rough", map_bottom, 2, 8);
	map->DrawMaterial("Earth-earth_dry", map_bottom, 2, 8);
	map->DrawMaterial("Earth-earth_midsoil", map_bottom, 4, 6);
	
	// Draw a labyrinth using nodes and connections and make it out of tunnel.
	var nodes = FindVeinNodes(map, size + 12, size / 2);
	var connections = FindNodeConnections(nodes, 24);
	for (var con in connections)
		map->Draw("Tunnel", con);
	
	// Make three out of the five gems nodes contain gems and protect with granite.
	var cnt = 0, i = 0;
	while (cnt < 3 && i < 5)
	{
		var node = nodes[i];
		i++;
		if (node.conn_count == 0)
			continue;
		var tunnel = node.tunnels[0];
		var y_constraint = {Algo = MAPALGO_Rect, X = 0, Y = node.Y - 3, Wdt = wdt, Hgt = 12};
		var gem_border = {Algo = MAPALGO_And, Op = [{Algo = MAPALGO_Border, Left = 1, Right = 1, Op = tunnel}, {Algo = MAPALGO_Rect, X = 0, Y = node.Y - 4, Wdt = wdt, Hgt = 10}]};
		var granite_border = {Algo = MAPALGO_And, Op = [{Algo = MAPALGO_Border, Left = 1, Right = 1, Op = tunnel}, {Algo = MAPALGO_Rect, X = 0, Y = node.Y - 6, Wdt = wdt, Hgt = 2}]};
		map->Draw(["Ruby", "Amethyst"][Random(2)], gem_border);
		map->Draw("Granite", granite_border);
		cnt++;
	}
	
	// Replace the tunnels with water up to a certain level.
	var water_level = 5 * size / 6;
	var tunnels = Duplicate("Tunnel");
	var tunnels_algo = {Algo = MAPALGO_Layer, Layer = tunnels};
	tunnels_algo = {Algo = MAPALGO_And, Op = [tunnels_algo, {Algo = MAPALGO_Rect, X = 0, Y = hgt - water_level, Wdt = wdt, Hgt = water_level}]}; 
	map->Draw("Water", tunnels_algo);	
    return;
}


/*-- Nodes and Connections --*/

public func FindVeinNodes(proplist map, int size, nr_nodes)
{
	var wdt = map.Wdt;
    var hgt = map.Hgt;
	
	// Prepare a mask out of the map to search for nodes.
	var mask = map->CreateLayer();
	mask->Draw("Rock");

	// Array for the nodes and definition for a minimum distance.	
	var nodes = [];
	var node_dist = 8;

	// The first five nodes are at the bottom of the map and are not to be connected.
	for (var i = 0; i < 5; i++)
	{
		var node = {};
		if (!mask->FindPosition(node, "Rock", [4, hgt - 10, wdt - 8, 6]))
			continue;
		mask->Draw("Tunnel", {Algo = MAPALGO_Ellipsis, X = node.X, Y = node.Y, Wdt = node_dist, Hgt = node_dist});
		node.is_gem = true;
		node.tunnels = [];
		PushBack(nodes, node);	
	}
	
	// The other nodes are scattered randomly in the lower areas
	for (var i = 5; i < nr_nodes; i++)
	{
		var node = {};
		if (!mask->FindPosition(node, "Rock", [4, hgt - size + 4, wdt - 8, size - 18]))
			continue;
		mask->Draw("Tunnel", {Algo = MAPALGO_Ellipsis, X = node.X, Y = node.Y, Wdt = node_dist, Hgt = node_dist});
		node.tunnels = [];
		PushBack(nodes, node);
	}
	return nodes;
}

public func FindNodeConnections(array nodes, int max_length)
{
	var connections = [];
	for (var node in nodes)
		node.conn_count = 0;	
	for (var i = 0; i < GetLength(nodes) - 1; i++)
	{
		var from_node = nodes[i];
		for (var j = i + 1; j < GetLength(nodes); j++)
		{
			var to_node = nodes[j];
			// Check for the maximum connections per cave.
			if (from_node.conn_count >= 4 || to_node.conn_count >= 4)
				continue;
			// Check for two gem nodes which may not connect.
			if (from_node.is_gem && to_node.is_gem)
				continue;
			// Gem nodes may have at most one connection.
			if ((from_node.is_gem && from_node.conn_count >= 1) || (to_node.is_gem && to_node.conn_count >= 1))
				continue;
			// Cave line parameters.
			var fx = from_node.X;
			var fy = from_node.Y;
			var tx = to_node.X;
			var ty = to_node.Y;
			// Check for the maximum line distance.
			if (Distance(fx, fy, tx, ty) > max_length)
				continue;
			// Check for overlap in existing connections.
			var has_overlap = false;
			for (var line in connections)
				if (IsLineOverlap(fx, fy, tx, ty, line.Op.X[0], line.Op.Y[0], line.Op.X[1], line.Op.Y[1]))	
				{
					has_overlap = true;
					break;
				}
			if (has_overlap)
				continue;
			var tunnel_width = 2;
			if (!Random(4))
				tunnel_width = 3;
			var tunnel = {Algo = MAPALGO_Polygon, X = [fx, tx], Y = [fy, ty], Wdt = tunnel_width, Open = 1, Empty = 1};
			tunnel = {Algo = MAPALGO_Turbulence, Amplitude = 5, Scale = 3, Seed = Random(65536), Op = tunnel};
			PushBack(connections, tunnel);
			PushBack(from_node.tunnels, tunnel);
			PushBack(to_node.tunnels, tunnel);
			nodes[i].conn_count++;
			nodes[j].conn_count++;
		}
	}
	return connections;
}


/*-- Helper Functions --*/

public func IsLineOverlap(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	// Same starting point is not overlapping.
	if ((x1 == x3 && y1 == y3) || (x1 == x4 && y1 == y4) || (x2 == x3 && y2 == y3) || (x2 == x4 && y2 == y4))
		return false;	
	
	// Check if line from x1,y1 to x2,y2 crosses the line from x3,y3 to x4,y4
	var d1x = x2 - x1, d1y = y2 - y1, d2x = x4 - x3, d2y = y4 - y3, d3x = x3 - x1, d3y = y3 - y1;
	var a = d1y * d3x - d1x * d3y;
	var b = d2y * d3x - d2x * d3y;
	var c = d2y * d1x - d2x * d1y;
	if (!c) 
		return !a && Inside(x3, x1, x2) && Inside(y3, y1, y2); // lines are parallel
	return a * c >= 0 && !(a * a / (c * c + 1)) && b * c >= 0 && !(b * b/(c * c + 1));
}

// Draws some material inside an island.
public func DrawMaterial(string mat, proplist onto_mask, int speck_size, int ratio)
{
	if (!speck_size)
		speck_size = 4;
	if (!ratio)
		ratio = 15;
	// Use random checker algorithm to draw patches of the material. 
	var rnd_checker = {Algo = MAPALGO_RndChecker, Ratio = ratio, Wdt = speck_size, Hgt = speck_size};
	rnd_checker = {Algo = MAPALGO_Turbulence, Iterations = 4, Op = rnd_checker};
	var algo = {Algo = MAPALGO_And, Op = [onto_mask, rnd_checker]};
	return Draw(mat, algo);
}