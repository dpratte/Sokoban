-- Event handling for the game canvas.
print("Loading event handlers from events.lua")
canvas_invalidated = true

function skb_OnEvent(e)
	print("Event: " .. e)
	if e == "CANVAS_CLOSED" then
		skb_quit()	
	end
	return 0
end

-- Game world update called.
-- Returns a boolean determining if a redraw must be done on screen.
function skb_OnUpdate(e)
	if canvas_invalidated then
		canvas_invalidated = false
		return true
	end
	return false
end
print("Finished loading event handlers from events.lua")
