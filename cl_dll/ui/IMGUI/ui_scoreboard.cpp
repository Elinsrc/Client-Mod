#include "ui_scoreboard.h"
#include "imgui.h"
#include "hud.h"
#include "cl_util.h"
#include "keydefs.h"
#include "imgui_backend.h"
#include <algorithm>

struct PlayerScore {
    int index;
    int score;
};

cvar_t *ui_imgui_scoreboard;

bool CImGuiScoreboard::m_ShowWindow = false;

void CImGuiScoreboard::Initialize()
{
    ui_imgui_scoreboard = CVAR_CREATE( "ui_imgui_scoreboard", "0", FCVAR_ARCHIVE );
}

void CImGuiScoreboard::VidInitialize()
{
}

void CImGuiScoreboard::Terminate()
{
}

void CImGuiScoreboard::Think()
{
}

void CImGuiScoreboard::DrawScoreboard()
{
    gHUD.GetAllPlayersInfo();

    int m_iPlayerNum = 0;
    for (int i = 1; i < MAX_PLAYERS; i++)
        if (g_PlayerInfoList[i].thisplayer) { m_iPlayerNum = i; break; }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 40.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(12, 2));

    float padding_y = 20.0f;
    float sb_width = 700.0f;
    float sb_height = ScreenHeight - 2 * padding_y;

    float sb_x = (ScreenWidth - sb_width) / 2;
    float sb_y = padding_y;

    ImGui::SetNextWindowPos(ImVec2(sb_x, sb_y));
    ImGui::SetNextWindowSize(ImVec2(sb_width, sb_height));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.02f, 0.02f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.05f, 0.05f, 0.05f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.08f, 0.08f, 0.08f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.65f, 0.0f, 1.0f));

    if (ImGui::Begin("##Scoreboard", &m_ShowWindow,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoInputs))
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 win_pos = ImGui::GetWindowPos();
        ImVec2 win_size = ImGui::GetWindowSize();

        draw_list->AddRect(win_pos, ImVec2(win_pos.x + win_size.x, win_pos.y + win_size.y), IM_COL32(255, 165, 0, 240), 40.0f, 0, 2.0f);

        std::vector<PlayerScore> sortedPlayers;

        for (int i = 1; i < MAX_PLAYERS; i++)
        {
            if (g_PlayerInfoList[i].name)
            {
                sortedPlayers.push_back({i, g_PlayerExtraInfo[i].frags});
            }
        }

        std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const PlayerScore& a, const PlayerScore& b){return a.score > b.score;});

        char player_count[256];
        sprintf(player_count, "%d/%d", get_player_count(), gEngfuncs.GetMaxClients());

        m_ImguiUtils.TextWithColorCodes(gHUD.m_szServerName);

        ImGui::SameLine();

        float right_text_width = ImGui::CalcTextSize(player_count).x;
        float full_width = ImGui::GetContentRegionAvail().x;
        float cursor_x = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX(cursor_x + full_width - right_text_width);

        ImGui::Text("%s", player_count);

        ImGui::Spacing();

        char map_name[64];
        get_map_name(map_name, ARRAYSIZE(map_name));
        ImGui::Text("%s", map_name);

        if (ImGui::BeginTable("ScoreboardTableInvisible", 5, ImGuiTableFlags_SizingFixedFit, ImVec2(0, 480)))
        {
            ImGui::TableSetupColumn("PLAYERS", ImGuiTableColumnFlags_WidthStretch, 0.0f);
            ImGui::TableSetupColumn("MODEL", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("SCORE", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("DEATS", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("PING", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            ImVec2 line_start = ImGui::GetCursorScreenPos();
            line_start.x = win_pos.x + ImGui::GetStyle().WindowPadding.x;
            line_start.y -= 2.0f;
            ImVec2 line_end = ImVec2(win_pos.x + win_size.x - ImGui::GetStyle().WindowPadding.x, line_start.y );
            draw_list->AddLine(line_start, line_end, IM_COL32(100, 100, 100, 150), 1.0f);

            ImGui::Spacing();

            for (const auto& player : sortedPlayers)
            {
                int i = player.index;
                bool isCurrentPlayer = (i == m_iPlayerNum);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);

                if (isCurrentPlayer)
                {
                    ImVec2 row_min = ImGui::GetCursorScreenPos();
                    row_min.x = win_pos.x + ImGui::GetStyle().WindowPadding.x;
                    ImVec2 row_max = ImVec2( win_pos.x + win_size.x - ImGui::GetStyle().WindowPadding.x, row_min.y + ImGui::GetTextLineHeight());
                    draw_list->AddRectFilled(row_min, row_max, IM_COL32(30, 80, 180, 100), 0.0f);
                }

                m_ImguiUtils.TextWithColorCodes(g_PlayerInfoList[i].name);

                ImGui::TableSetColumnIndex(1);
                m_ImguiUtils.DrawModelName(g_PlayerInfoList[i].topcolor, g_PlayerInfoList[i].bottomcolor, g_PlayerInfoList[i].model);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", g_PlayerExtraInfo[i].frags);

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%d", g_PlayerExtraInfo[i].deaths);

                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%d", g_PlayerInfoList[i].ping);
            };
            ImGui::EndTable();
        }
        ImGui::End();
    }
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(6);
}

void CImGuiScoreboard::Draw()
{
    if (m_ShowWindow) {
        DrawScoreboard();
    }
}

bool CImGuiScoreboard::Active()
{
    return m_ShowWindow;
}

bool CImGuiScoreboard::CursorRequired()
{
    return false;
}

bool CImGuiScoreboard::HandleKey(bool keyDown, int keyNumber, const char *bindName)
{
    return true;
}
