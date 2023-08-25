#pragma once

namespace discord_integration
{
	/**
	 * Initializes the Discord integration. Call this once on game start.
	 */
	void initialize();

	/**
	 * Shuts down the Discord integration. Call this once on game end.
	 */
	void shutdown();

	/**
	 * Sets the Discord state.
	 */
	void set_spectating(bool spectating);

	/**
	 * Sets the current gamemode.
	 */
	void set_gamemode(std::string gamemode);

	/**
	 * Call this in HUD_UpdateClientData.
	 */
	void on_update_client_data();

	/**
	 * Call this in HUD_Frame.
	 */
	void on_frame();

	/**
	 * Call this when a player joins or leaves.
	 */
	void on_player_count_update();
}