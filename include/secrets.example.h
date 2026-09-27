/*
 * Secrets Configuration Template - Smart Lighting Telemetry
 *
 * Descrição:
 * Modelo de credenciais para autenticação de rede Wi-Fi e Arduino Cloud.
 * Copie este arquivo para 'include/secrets.h' e insira suas credenciais reais.
 * O arquivo 'secrets.h' é ignorado pelo Git para evitar vazamento de segredos.
 */

#pragma once

namespace config {
constexpr char wifi_ssid[] = "MOCK_WIFI_SSID";
constexpr char wifi_pass[] = "MOCK_WIFI_PASSWORD";
constexpr char device_login_name[] = "00000000-0000-0000-0000-000000000000";
constexpr char device_key[] = "mock_secret_key";
constexpr char thing_id[] = "00000000-0000-0000-0000-000000000000";
}  // namespace config
