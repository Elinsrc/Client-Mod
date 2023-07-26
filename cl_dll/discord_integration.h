#pragma once

namespace discord_integration
{
	/**
	 * Possible game states.
	 */
	enum class game_state
	{
		NOT_PLAYING = 0,
		PLAYING,
		IN_A_MATCH,
		SPECTATING,
	};

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
	void set_state(game_state new_state);

	/**
	 * Sets the current gamemode.
	 */
	void set_gamemode(std::string new_gamemode);

	/*
	 * Sets the match time data.
	 */
	void set_time_data(int seconds_total, int seconds_passed);

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
