#pragma once

#include "kernel/support/miscellaneous/shared.hpp"
#include "kernel/support/wwise/sound_bank/definition.hpp"

namespace Sen::Kernel::Support::WWise::SoundBank
{
    using namespace Definition;

    using namespace Sen::Kernel::Support::Miscellaneous::Shared;

    struct Common
    {

        inline static constexpr auto k_hierarchy_event_type_string = "event"_sv;

        inline static constexpr auto k_data_block_padding_size = 0x10_size;

        inline static auto k_bitset_boolean_fill = false;

        struct ChunkSignFlag
        {

            inline static constexpr auto bkhd = "BKHD"_sv;

            inline static constexpr auto didx = "DIDX"_sv;

            inline static constexpr auto data = "DATA"_sv;

            inline static constexpr auto init = "INIT"_sv;

            inline static constexpr auto stmg = "STMG"_sv;

            inline static constexpr auto hirc = "HIRC"_sv;

            inline static constexpr auto stid = "STID"_sv;

            inline static constexpr auto envs = "ENVS"_sv;

            inline static constexpr auto plat = "PLAT"_sv;
        };

        struct ChunkSign
        {
            std::string_view id;
            uint32_t pos;
            uint32_t size;
        };

        inline static auto exchange_chuck_sign(
            DataStreamView &stream,
            ChunkSign &value) -> void
        {
            value.id = stream.readString(4);
            value.size = stream.readUint32();
            return;
        }

        inline static auto exchange_chuck_sign(
            ChunkSign const &value,
            DataStreamView &stream) -> void
        {
            stream.writeString(value.id);
            stream.writeUint32(value.size);
            return;
        }

        inline static auto fnv_hash(
            const std::string &name) -> uint32_t
        {
            auto hash = uint32_t{2166136261};
            for (const auto &c : name)
            {
                hash = (hash * 16777619) ^ static_cast<uint8_t>(tolower(c));
            }
            return hash;
        }

        enum class EventActionCommonPropertyType : uint8_t
        {
            delay,
            fade_time,
            probability
        };

        enum class ModulatorCommonPropertyType : uint8_t
        { // >= 112
            scope,
            trigger_on,
            depth,
            frequency,
            waveform,
            smoothing,
            pulse_width_modulation,
            attack,
            initial_phase_offset,
            attack_time,
            attack_curve,
            decay_time,
            sustain_level,
            release_time,
            sustain_time,
            initial_delay,
            duration,
            loop,
            playback_rate,
            stop_playback
        };

        enum class AudioCommonPropertyType : uint8_t
        {
            bus_volume,
            output_bus_volume,
            output_bus_low_pass_filter,
            voice_volume,
            voice_pitch,
            voice_low_pass_filter,
            game_defined_auxiliary_send_volume,
            user_defined_auxiliary_send_volume_0,
            user_defined_auxiliary_send_volume_1,
            user_defined_auxiliary_send_volume_2,
            user_defined_auxiliary_send_volume_3,
            positioning_center_percent,
            positioning_speaker_panning_x,
            positioning_speaker_panning_y,
            playback_priority_value,
            playback_priority_offset_at_maximum_distance,
            playback_loop,
            motion_volume_offset,
            motion_low_pass_filter,
            voice_volume_make_up_gain,
            hdr_threshold,
            hdr_ratio,
            hdr_release_time,
            hdr_window_tap_output_game_parameter_id,
            hdr_window_tap_output_game_parameter_minimum,
            hdr_window_tap_output_game_parameter_maximum,
            hdr_envelope_tracking_active_range,
            playback_initial_delay,
            output_bus_high_pass_filter,
            voice_high_pass_filter,
            midi_note_tracking_root_note,
            midi_event_play_on,
            midi_transformation_transposition,
            midi_transformation_velocity_offset,
            midi_filter_key_range_minimum,
            midi_filter_key_range_maximum,
            midi_filter_velocity_minimum,
            midi_filter_velocity_maximum,
            midi_filter_channel,
            midi_clip_tempo_source,
            midi_target_id,
            playback_speed,
            mixer_id,
            game_defined_auxiliary_send_low_pass_filter,
            game_defined_auxiliary_send_high_pass_filter,
            user_defined_auxiliary_send_low_pass_filter_0,
            user_defined_auxiliary_send_low_pass_filter_1,
            user_defined_auxiliary_send_low_pass_filter_2,
            user_defined_auxiliary_send_low_pass_filter_3,
            user_defined_auxiliary_send_high_pass_filter_0,
            user_defined_auxiliary_send_high_pass_filter_1,
            user_defined_auxiliary_send_high_pass_filter_2,
            user_defined_auxiliary_send_high_pass_filter_3,
            positioning_listener_routing_speaker_panning_division_spatialization_mix,
            positioning_listener_routing_attenuation_id,
            early_reflection_auxiliary_send_volume,
            positioning_speaker_panning_z
        };

        template <typename Type>
            requires std::is_enum<Type>::value && true
        struct CommonPropertyMap
        {
            std::vector<std::pair<Type, std::any>> regular;
            std::vector<std::pair<Type, std::array<std::any, 2>>> randomizer;
        };

        // Type is uint8_t || enum
        template <typename InputType, typename ReturnType>
            requires((std::is_enum<InputType>::value || std::is_same<InputType, uint8_t>::value)) && (std::is_enum<ReturnType>::value || std::is_same<ReturnType, uint8_t>::value || std::is_same<ReturnType, size_t>::value) && true
        inline static auto exchange_enumeration(
            InputType const &data) -> ReturnType
        {
            // static_assert(sizeof...(InputType) == 1 || sizeof...(InputType) == 0, "Expected 0 or 1 argument only");
            static_assert(std::is_enum<InputType>::value || std::is_enum<ReturnType>::value, "Must be one enumeration type");
            if constexpr (std::is_same<InputType, Curve>::value || std::is_same<ReturnType, Curve>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 4_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, Curve>, 10> value_package = {{
                        {9, Curve::constant},
                        {4, Curve::linear},
                        {5, Curve::s},
                        {3, Curve::s_inverted},
                        {1, Curve::sine},
                        {7, Curve::sine_reciprocal},
                        {2, Curve::logarithmic_1dot41},
                        {0, Curve::logarithmic_3dot0},
                        {6, Curve::exponential_1dot41},
                        {8, Curve::exponential_3dot0}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioSourceType>::value || std::is_same<ReturnType, AudioSourceType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    if (k_version < 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioSourceType>, 3> value_package = {{
                            {0, AudioSourceType::embedded},
                            {1, AudioSourceType::streamed},
                            {2, AudioSourceType::streamed_prefetched}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, AudioSourceType>, 3> value_package = {{
                            {0, AudioSourceType::embedded},
                            {2, AudioSourceType::streamed},
                            {1, AudioSourceType::streamed_prefetched}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, MusicTrackClipCurveItemType>::value || std::is_same<ReturnType, MusicTrackClipCurveItemType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 3_size;
                }
                else
                {
                    if (k_version < 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, MusicTrackClipCurveItemType>, 4> value_package = {{
                            {0, MusicTrackClipCurveItemType::voice_volume},
                            {1, MusicTrackClipCurveItemType::voice_low_pass_filter},
                            {2, MusicTrackClipCurveItemType::clip_fade_in},
                            {3, MusicTrackClipCurveItemType::clip_fade_out}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, MusicTrackClipCurveItemType>, 5> value_package = {{
                            {0, MusicTrackClipCurveItemType::voice_volume},
                            {1, MusicTrackClipCurveItemType::voice_low_pass_filter},
                            {2, MusicTrackClipCurveItemType::voice_high_pass_filter},
                            {3, MusicTrackClipCurveItemType::clip_fade_in},
                            {4, MusicTrackClipCurveItemType::clip_fade_out}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, TimePoint>::value || std::is_same<ReturnType, TimePoint>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return (k_version < 140_ui) ? 3_size : 4_size;
                }
                else
                {
                    if (k_version < 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, TimePoint>, 8> value_package = {{
                            {0, TimePoint::immediate},
                            {1, TimePoint::next_grid},
                            {2, TimePoint::next_bar},
                            {3, TimePoint::next_beat},
                            {4, TimePoint::next_cue},
                            {5, TimePoint::custom_cue},
                            {6, TimePoint::entry_cue},
                            {7, TimePoint::exit_cue}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, TimePoint>, 9> value_package = {{
                            {0, TimePoint::immediate},
                            {1, TimePoint::next_grid},
                            {2, TimePoint::next_bar},
                            {3, TimePoint::next_beat},
                            {4, TimePoint::next_cue},
                            {5, TimePoint::custom_cue},
                            {6, TimePoint::entry_cue},
                            {7, TimePoint::exit_cue},
                            {9, TimePoint::last_exit_position}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, CoordinateMode>::value || std::is_same<ReturnType, CoordinateMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, CoordinateMode>, 3> value_package = {{
                        {0, CoordinateMode::linear},
                        {2, CoordinateMode::scaled},
                        {3, CoordinateMode::scaled_3}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, PropertyCategory>::value || std::is_same<ReturnType, PropertyCategory>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return (k_version < 128_ui) ? 2_size : 3_size;
                }
                else
                {
                    if (k_version < 128_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, PropertyCategory>, 4> value_package = {{
                            {0, PropertyCategory::unidirectional},
                            {1, PropertyCategory::bidirectional},
                            {2, PropertyCategory::bidirectional_ranged},
                            {3, PropertyCategory::boolean}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 145_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, PropertyCategory>, 4> value_package = {{
                            {1, PropertyCategory::unidirectional},
                            {2, PropertyCategory::bidirectional},
                            {3, PropertyCategory::bidirectional_ranged},
                            {4, PropertyCategory::boolean}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, PropertyCategory>, 5> value_package = {{
                            {1, PropertyCategory::unidirectional},
                            {2, PropertyCategory::bidirectional},
                            {3, PropertyCategory::bidirectional_ranged},
                            {6, PropertyCategory::boolean},
                            {4, PropertyCategory::unknown_6}
                        }};
                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, ParameterCategory>::value || std::is_same<ReturnType, ParameterCategory>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return (k_version < 145_ui) ? 2_size : 3_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        if (k_version < 145_ui)
                        {
                            constexpr std::array<std::pair<uint8_t, ParameterCategory>, 3> value_package = {{
                                {0, ParameterCategory::game_parameter},
                                {1, ParameterCategory::midi_parameter},
                                {2, ParameterCategory::modulator}
                            }};
                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                        else
                        {
                            constexpr std::array<std::pair<uint8_t, ParameterCategory>, 3> value_package = {{ // NOTE: This was previously size 9, but only 3 elements were defined. Corrected to 3.
                                {0, ParameterCategory::game_parameter},
                                {1, ParameterCategory::midi_parameter},
                                {4, ParameterCategory::modulator}
                            }};
                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlayType>::value || std::is_same<ReturnType, AudioPlayType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlayType>, 2> value_package = {{
                        {0, AudioPlayType::sequence},
                        {1, AudioPlayType::random}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlayMode>::value || std::is_same<ReturnType, AudioPlayMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlayMode>, 2> value_package = {{
                        {0, AudioPlayMode::step},
                        {1, AudioPlayMode::continuous}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPositioningSettingListenerRoutingPositionSourceMode>::value || std::is_same<ReturnType, AudioPositioningSettingListenerRoutingPositionSourceMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    if (k_version < 132_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioPositioningSettingListenerRoutingPositionSourceMode>, 2> value_package = {{
                            {0, AudioPositioningSettingListenerRoutingPositionSourceMode::user_defined},
                            {1, AudioPositioningSettingListenerRoutingPositionSourceMode::game_defined}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, AudioPositioningSettingListenerRoutingPositionSourceMode>, 3> value_package = {{
                            {0, AudioPositioningSettingListenerRoutingPositionSourceMode::emitter},
                            {1, AudioPositioningSettingListenerRoutingPositionSourceMode::emitter_with_automation},
                            {2, AudioPositioningSettingListenerRoutingPositionSourceMode::listener_with_automation}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPositioningSettingListenerRoutingSpatialization>::value || std::is_same<ReturnType, AudioPositioningSettingListenerRoutingSpatialization>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 128_ui)
                    {
                        return 3_size;
                    }
                }
                else
                {
                    if (k_version >= 128_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioPositioningSettingListenerRoutingSpatialization>, 3> value_package = {{
                            {0, AudioPositioningSettingListenerRoutingSpatialization::none},
                            {1, AudioPositioningSettingListenerRoutingSpatialization::position},
                            {2, AudioPositioningSettingListenerRoutingSpatialization::position_and_orientation}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPositioningSettingSpeakerPanningMode>::value || std::is_same<ReturnType, AudioPositioningSettingSpeakerPanningMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 132_ui)
                    {
                        return 2_size;
                    }
                }
                else
                {
                    if (k_version >= 132_ui)
                    {
                        if (k_version < 140_ui)
                        {
                            constexpr std::array<std::pair<uint8_t, AudioPositioningSettingSpeakerPanningMode>, 2> value_package = {{
                                {0, AudioPositioningSettingSpeakerPanningMode::direct_assignment},
                                {1, AudioPositioningSettingSpeakerPanningMode::balance_fade}
                            }};
                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                        else
                        {
                            constexpr std::array<std::pair<uint8_t, AudioPositioningSettingSpeakerPanningMode>, 3> value_package = {{
                                {0, AudioPositioningSettingSpeakerPanningMode::direct_assignment},
                                {1, AudioPositioningSettingSpeakerPanningMode::balance_fade},
                                {2, AudioPositioningSettingSpeakerPanningMode::steering}
                            }};
                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPositioningSettingType>::value || std::is_same<ReturnType, AudioPositioningSettingType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version < 132_ui)
                    {
                        return 1_size;
                    }
                }
                else
                {
                    if (k_version < 132_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioPositioningSettingType>, 2> value_package = {{
                            {0, AudioPositioningSettingType::two_dimension},
                            {1, AudioPositioningSettingType::three_dimension}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, BusAutomaticDuckingSettingBusTarget>::value || std::is_same<ReturnType, BusAutomaticDuckingSettingBusTarget>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 3_size;
                }
                else
                {
                    if (k_version < 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, BusAutomaticDuckingSettingBusTarget>, 2> value_package = {{
                            {0, BusAutomaticDuckingSettingBusTarget::voice_volume},
                            {4, BusAutomaticDuckingSettingBusTarget::bus_volume}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, BusAutomaticDuckingSettingBusTarget>, 2> value_package = {{
                            {0, BusAutomaticDuckingSettingBusTarget::voice_volume},
                            {5, BusAutomaticDuckingSettingBusTarget::bus_volume}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, MusicTransitionSettingJumpMode>::value || std::is_same<ReturnType, MusicTransitionSettingJumpMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 134_ui)
                    {
                        return 2_size;
                    }
                }
                else
                {
                    if (k_version >= 134_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, MusicTransitionSettingJumpMode>, 4> value_package = {{
                            {0, MusicTransitionSettingJumpMode::start},
                            {1, MusicTransitionSettingJumpMode::specific},
                            {3, MusicTransitionSettingJumpMode::next},
                            {2, MusicTransitionSettingJumpMode::last_played}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, MusicTransitionSettingSynchronizeMode>::value || std::is_same<ReturnType, MusicTransitionSettingSynchronizeMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, MusicTransitionSettingSynchronizeMode>, 4> value_package = {{
                        {0, MusicTransitionSettingSynchronizeMode::entry_cue},
                        {2, MusicTransitionSettingSynchronizeMode::random_cue},
                        {3, MusicTransitionSettingSynchronizeMode::custom_cue},
                        {1, MusicTransitionSettingSynchronizeMode::same_time_as_playing_segment}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, BusHDRSettingDynamicReleaseMode>::value || std::is_same<ReturnType, BusHDRSettingDynamicReleaseMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 88_ui)
                    {
                        return 1_size;
                    }
                }
                else
                {
                    if (k_version >= 88_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, BusHDRSettingDynamicReleaseMode>, 2> value_package = {{
                            {0, BusHDRSettingDynamicReleaseMode::linear},
                            {1, BusHDRSettingDynamicReleaseMode::exponential}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, SoundMIDISettingEventPlayOn>::value || std::is_same<ReturnType, SoundMIDISettingEventPlayOn>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return 2_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, SoundMIDISettingEventPlayOn>, 2> value_package = {{
                            {0, SoundMIDISettingEventPlayOn::note_on},
                            {2, SoundMIDISettingEventPlayOn::note_off}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, MusicMIDISettingClipTempoSource>::value || std::is_same<ReturnType, MusicMIDISettingClipTempoSource>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return 1_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, MusicMIDISettingClipTempoSource>, 2> value_package = {{
                            {0, MusicMIDISettingClipTempoSource::hierarchy},
                            {1, MusicMIDISettingClipTempoSource::file}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlaybackLimitSettingScope>::value || std::is_same<ReturnType, AudioPlaybackLimitSettingScope>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlaybackLimitSettingScope>, 2> value_package = {{
                        {0, AudioPlaybackLimitSettingScope::per_game_object},
                        {1, AudioPlaybackLimitSettingScope::globally}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlaybackLimitSettingWhenPriorityIsEqual>::value || std::is_same<ReturnType, AudioPlaybackLimitSettingWhenPriorityIsEqual>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlaybackLimitSettingWhenPriorityIsEqual>, 2> value_package = {{
                        {0, AudioPlaybackLimitSettingWhenPriorityIsEqual::discard_oldest_instance},
                        {1, AudioPlaybackLimitSettingWhenPriorityIsEqual::discard_newest_instance}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlaybackLimitSettingWhenLimitIsReached>::value || std::is_same<ReturnType, AudioPlaybackLimitSettingWhenLimitIsReached>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlaybackLimitSettingWhenLimitIsReached>, 2> value_package = {{
                        {0, AudioPlaybackLimitSettingWhenLimitIsReached::kill_voice},
                        {1, AudioPlaybackLimitSettingWhenLimitIsReached::use_virtual_voice_setting}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioVirtualVoiceSettingBehavior>::value || std::is_same<ReturnType, AudioVirtualVoiceSettingBehavior>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    if (k_version < 140_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioVirtualVoiceSettingBehavior>, 3> value_package = {{
                            {0, AudioVirtualVoiceSettingBehavior::continue_to_play},
                            {1, AudioVirtualVoiceSettingBehavior::kill_voice},
                            {2, AudioVirtualVoiceSettingBehavior::send_to_virtual_voice}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, AudioVirtualVoiceSettingBehavior>, 4> value_package = {{
                            {0, AudioVirtualVoiceSettingBehavior::continue_to_play},
                            {1, AudioVirtualVoiceSettingBehavior::kill_voice},
                            {2, AudioVirtualVoiceSettingBehavior::send_to_virtual_voice},
                            {2, AudioVirtualVoiceSettingBehavior::kill_if_finite_else_virtual} // Note: Original had '2' for the last value, kept as is.
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioVirtualVoiceSettingOnReturnToPhysical>::value || std::is_same<ReturnType, AudioVirtualVoiceSettingOnReturnToPhysical>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioVirtualVoiceSettingOnReturnToPhysical>, 3> value_package = {{
                        {0, AudioVirtualVoiceSettingOnReturnToPhysical::play_from_beginning},
                        {1, AudioVirtualVoiceSettingOnReturnToPhysical::play_from_elapsed_time},
                        {2, AudioVirtualVoiceSettingOnReturnToPhysical::resume}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioAssociationSettingMode>::value || std::is_same<ReturnType, AudioAssociationSettingMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioAssociationSettingMode>, 2> value_package = {{
                        {0, AudioAssociationSettingMode::best_match},
                        {1, AudioAssociationSettingMode::weighted}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlayTypeRandomType>::value || std::is_same<ReturnType, AudioPlayTypeRandomType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlayTypeRandomType>, 2> value_package = {{
                        {0, AudioPlayTypeRandomType::standard},
                        {1, AudioPlayTypeRandomType::shuffle}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlayTypeSequenceAtEndOfPlaylist>::value || std::is_same<ReturnType, AudioPlayTypeSequenceAtEndOfPlaylist>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlayTypeSequenceAtEndOfPlaylist>, 2> value_package = {{
                        {0, AudioPlayTypeSequenceAtEndOfPlaylist::restart},
                        {1, AudioPlayTypeSequenceAtEndOfPlaylist::play_in_reserve_order}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioPlayModeContinuousTransitionType>::value || std::is_same<ReturnType, AudioPlayModeContinuousTransitionType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 3_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, AudioPlayModeContinuousTransitionType>, 6> value_package = {{
                        {0, AudioPlayModeContinuousTransitionType::none},
                        {1, AudioPlayModeContinuousTransitionType::xfade_amp},
                        {2, AudioPlayModeContinuousTransitionType::xfade_power},
                        {3, AudioPlayModeContinuousTransitionType::delay},
                        {4, AudioPlayModeContinuousTransitionType::sample_accurate},
                        {5, AudioPlayModeContinuousTransitionType::trigger_rate}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, GameParameterBindToBuiltInParameterMode>::value || std::is_same<ReturnType, GameParameterBindToBuiltInParameterMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return 3_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        if (k_version < 128_ui)
                        {
                            constexpr std::array<std::pair<uint8_t, GameParameterBindToBuiltInParameterMode>, 7> value_package = {{
                                {0, GameParameterBindToBuiltInParameterMode::none},
                                {1, GameParameterBindToBuiltInParameterMode::distance},
                                {2, GameParameterBindToBuiltInParameterMode::azimuth},
                                {3, GameParameterBindToBuiltInParameterMode::elevation},
                                {4, GameParameterBindToBuiltInParameterMode::object_to_listener_angle},
                                {5, GameParameterBindToBuiltInParameterMode::obstruction},
                                {6, GameParameterBindToBuiltInParameterMode::occlusion}
                            }};

                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                        else
                        {
                            constexpr std::array<std::pair<uint8_t, GameParameterBindToBuiltInParameterMode>, 9> value_package = {{
                                {0, GameParameterBindToBuiltInParameterMode::none},
                                {1, GameParameterBindToBuiltInParameterMode::distance},
                                {2, GameParameterBindToBuiltInParameterMode::azimuth},
                                {3, GameParameterBindToBuiltInParameterMode::elevation},
                                {4, GameParameterBindToBuiltInParameterMode::emitter_cone},
                                {5, GameParameterBindToBuiltInParameterMode::obstruction},
                                {6, GameParameterBindToBuiltInParameterMode::occlusion},
                                {7, GameParameterBindToBuiltInParameterMode::listener_cone},
                                {8, GameParameterBindToBuiltInParameterMode::diffraction}
                            }};

                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, GameParameterInterpolationMode>::value || std::is_same<ReturnType, GameParameterInterpolationMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return 2_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, GameParameterInterpolationMode>, 3> value_package = {{
                            {0, GameParameterInterpolationMode::none},
                            {1, GameParameterInterpolationMode::slew_rate},
                            {2, GameParameterInterpolationMode::filtering_over_time}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }

            if constexpr (std::is_same<InputType, EventActionProperty::ValueApplyMode>::value || std::is_same<ReturnType, EventActionProperty::ValueApplyMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, EventActionProperty::ValueApplyMode>, 2> value_package = {{
                        {1, EventActionProperty::ValueApplyMode::absolute},
                        {2, EventActionProperty::ValueApplyMode::relative}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, EventActionProperty::SeekType>::value || std::is_same<ReturnType, EventActionProperty::SeekType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, EventActionProperty::SeekType>, 2> value_package = {{
                        {0, EventActionProperty::SeekType::time},
                        {1, EventActionProperty::SeekType::percent}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }

            if constexpr (std::is_same<InputType, EventActionMode>::value || std::is_same<ReturnType, EventActionMode>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version < 125_ui)
                    {
                        return 3_size;
                    }
                    else
                    {
                        return 2_size;
                    }
                }
                else
                {
                    if (k_version < 125_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, EventActionMode>, 4> value_package = {{
                            {0, EventActionMode::none},
                            {1, EventActionMode::one},
                            {2, EventActionMode::all},
                            {4, EventActionMode::all_except}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, EventActionMode>, 3> value_package = {{
                            {0, EventActionMode::none},
                            {1, EventActionMode::one},
                            {2, EventActionMode::all}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, EventActionScope>::value || std::is_same<ReturnType, EventActionScope>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                    constexpr std::array<std::pair<uint8_t, EventActionScope>, 2> value_package = {{
                        {0, EventActionScope::global},
                        {1, EventActionScope::game_object}
                    }};

                    //---------------------------------
                    if constexpr (std::is_same<InputType, uint8_t>::value)
                    {
                        static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == data; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    if constexpr (std::is_enum<InputType>::value)
                    {
                        static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.second == data; });
                            it != value_package.end())
                        {
                            return it->first;
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, ModulatorScope>::value || std::is_same<ReturnType, ModulatorScope>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return 2_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, ModulatorScope>, 4> value_package = {{
                            {0, ModulatorScope::voice},
                            {1, ModulatorScope::note_or_event},
                            {2, ModulatorScope::game_object},
                            {3, ModulatorScope::global}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, ModulatorTriggerOn>::value || std::is_same<ReturnType, ModulatorTriggerOn>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return 2_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, ModulatorTriggerOn>, 2> value_package = {{
                            {0, ModulatorTriggerOn::play},
                            {2, ModulatorTriggerOn::note_off}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, ModulatorWaveform>::value || std::is_same<ReturnType, ModulatorWaveform>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        return 3_size;
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        if (k_version < 125_ui)
                        {
                            constexpr std::array<std::pair<uint8_t, ModulatorWaveform>, 5> value_package = {{
                                {0, ModulatorWaveform::sine},
                                {1, ModulatorWaveform::triangle},
                                {2, ModulatorWaveform::square},
                                {3, ModulatorWaveform::saw_up},
                                {4, ModulatorWaveform::saw_down}
                            }};

                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                        else
                        {
                            constexpr std::array<std::pair<uint8_t, ModulatorWaveform>, 6> value_package = {{
                                {0, ModulatorWaveform::sine},
                                {1, ModulatorWaveform::triangle},
                                {2, ModulatorWaveform::square},
                                {3, ModulatorWaveform::saw_up},
                                {4, ModulatorWaveform::saw_down},
                                {5, ModulatorWaveform::random}
                            }};

                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, SoundPlaylistContainerScope>::value || std::is_same<ReturnType, SoundPlaylistContainerScope>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 1_size;
                }
                else
                {
                        constexpr std::array<std::pair<uint8_t, SoundPlaylistContainerScope>, 2> value_package = {{
                            {0, SoundPlaylistContainerScope::game_object},
                            {1, SoundPlaylistContainerScope::global}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                }
            }
            if constexpr (std::is_same<InputType, VoiceFilterBehavior>::value || std::is_same<ReturnType, VoiceFilterBehavior>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 145_ui) {
                        return 1_size;
                    }
                }
                else
                {
                    if (k_version >= 145_ui) {
                        constexpr std::array<std::pair<uint8_t, VoiceFilterBehavior>, 2> value_package = {{
                            {0, VoiceFilterBehavior::sum_all_value},
                            {1, VoiceFilterBehavior::use_highest_value}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, MusicTrackTrackType>::value || std::is_same<ReturnType, MusicTrackTrackType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 2_size;
                }
                else
                {
                    if (k_version < 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, MusicTrackTrackType>, 3> value_package = {{
                            {0, MusicTrackTrackType::normal},
                            {1, MusicTrackTrackType::random_step},
                            {2, MusicTrackTrackType::sequence_step}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, MusicTrackTrackType>, 4> value_package = {{
                            {0, MusicTrackTrackType::normal},
                            {1, MusicTrackTrackType::random_step},
                            {2, MusicTrackTrackType::sequence_step},
                            {3, MusicTrackTrackType::switcher}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, HierarchyType>::value || std::is_same<ReturnType, HierarchyType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    return 8_size;
                }
                else
                {
                    if (k_version < 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, HierarchyType>, 19> value_package = {{
                            {0, HierarchyType::unknown},
                            {1, HierarchyType::stateful_property_setting},
                            {3, HierarchyType::event_action},
                            {4, HierarchyType::event},
                            {15, HierarchyType::dialogue_event},
                            {14, HierarchyType::attenuation},
                            {18, HierarchyType::effect},
                            {19, HierarchyType::source},
                            {8, HierarchyType::audio_bus},
                            {20, HierarchyType::auxiliary_audio_bus},
                            {2, HierarchyType::sound},
                            {5, HierarchyType::sound_playlist_container},
                            {6, HierarchyType::sound_switch_container},
                            {9, HierarchyType::sound_blend_container},
                            {7, HierarchyType::actor_mixer},
                            {11, HierarchyType::music_track},
                            {10, HierarchyType::music_segment},
                            {13, HierarchyType::music_playlist_container},
                            {12, HierarchyType::music_switch_container}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 128_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, HierarchyType>, 21> value_package = {{
                            {0, HierarchyType::unknown},
                            {1, HierarchyType::stateful_property_setting},
                            {3, HierarchyType::event_action},
                            {4, HierarchyType::event},
                            {15, HierarchyType::dialogue_event},
                            {14, HierarchyType::attenuation},
                            {21, HierarchyType::low_frequency_oscillator_modulator},
                            {22, HierarchyType::envelope_modulator},
                            {18, HierarchyType::effect},
                            {19, HierarchyType::source},
                            {8, HierarchyType::audio_bus},
                            {20, HierarchyType::auxiliary_audio_bus},
                            {2, HierarchyType::sound},
                            {5, HierarchyType::sound_playlist_container},
                            {6, HierarchyType::sound_switch_container},
                            {9, HierarchyType::sound_blend_container},
                            {7, HierarchyType::actor_mixer},
                            {11, HierarchyType::music_track},
                            {10, HierarchyType::music_segment},
                            {13, HierarchyType::music_playlist_container},
                            {12, HierarchyType::music_switch_container}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 132_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, HierarchyType>, 22> value_package = {{
                            {0, HierarchyType::unknown},
                            {1, HierarchyType::stateful_property_setting},
                            {3, HierarchyType::event_action},
                            {4, HierarchyType::event},
                            {15, HierarchyType::dialogue_event},
                            {14, HierarchyType::attenuation},
                            {19, HierarchyType::low_frequency_oscillator_modulator},
                            {20, HierarchyType::envelope_modulator},
                            {16, HierarchyType::effect},
                            {17, HierarchyType::source},
                            {21, HierarchyType::audio_device},
                            {8, HierarchyType::audio_bus},
                            {18, HierarchyType::auxiliary_audio_bus},
                            {2, HierarchyType::sound},
                            {5, HierarchyType::sound_playlist_container},
                            {6, HierarchyType::sound_switch_container},
                            {9, HierarchyType::sound_blend_container},
                            {7, HierarchyType::actor_mixer},
                            {11, HierarchyType::music_track},
                            {10, HierarchyType::music_segment},
                            {13, HierarchyType::music_playlist_container},
                            {12, HierarchyType::music_switch_container}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, HierarchyType>, 23> value_package = {{
                            {0, HierarchyType::unknown},
                            {1, HierarchyType::stateful_property_setting},
                            {3, HierarchyType::event_action},
                            {4, HierarchyType::event},
                            {15, HierarchyType::dialogue_event},
                            {14, HierarchyType::attenuation},
                            {19, HierarchyType::low_frequency_oscillator_modulator},
                            {20, HierarchyType::envelope_modulator},
                            {22, HierarchyType::time_modulator},
                            {16, HierarchyType::effect},
                            {17, HierarchyType::source},
                            {21, HierarchyType::audio_device},
                            {8, HierarchyType::audio_bus},
                            {18, HierarchyType::auxiliary_audio_bus},
                            {2, HierarchyType::sound},
                            {5, HierarchyType::sound_playlist_container},
                            {6, HierarchyType::sound_switch_container},
                            {9, HierarchyType::sound_blend_container},
                            {7, HierarchyType::actor_mixer},
                            {11, HierarchyType::music_track},
                            {10, HierarchyType::music_segment},
                            {13, HierarchyType::music_playlist_container},
                            {12, HierarchyType::music_switch_container}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }

            //------------------------------
            // CommonPropertyType
            if constexpr (std::is_same<InputType, EventActionCommonPropertyType>::value || std::is_same<ReturnType, EventActionCommonPropertyType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    try_assert(false, "common property type is not return size");
                }
                else
                {
                    if (k_version < 118_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, EventActionCommonPropertyType>, 3> value_package = {{
                            {14, EventActionCommonPropertyType::delay},
                            {15, EventActionCommonPropertyType::fade_time},
                            {16, EventActionCommonPropertyType::probability}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 150_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, EventActionCommonPropertyType>, 3> value_package = {{
                            {15, EventActionCommonPropertyType::delay},
                            {16, EventActionCommonPropertyType::fade_time},
                            {17, EventActionCommonPropertyType::probability}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, EventActionCommonPropertyType>, 3> value_package = {{
                            {57, EventActionCommonPropertyType::delay},
                            {58, EventActionCommonPropertyType::fade_time},
                            {59, EventActionCommonPropertyType::probability}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, ModulatorCommonPropertyType>::value || std::is_same<ReturnType, ModulatorCommonPropertyType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    if (k_version >= 112_ui)
                    {
                        try_assert(false, "common property type is not return size");
                    }
                }
                else
                {
                    if (k_version >= 112_ui)
                    {
                        if (k_version < 150_ui)
                        {
                            constexpr std::array<std::pair<uint8_t, ModulatorCommonPropertyType>, 20> value_package = {{
                                {0, ModulatorCommonPropertyType::scope},
                                {15, ModulatorCommonPropertyType::trigger_on},
                                {2, ModulatorCommonPropertyType::depth},
                                {4, ModulatorCommonPropertyType::frequency},
                                {5, ModulatorCommonPropertyType::waveform},
                                {6, ModulatorCommonPropertyType::smoothing},
                                {7, ModulatorCommonPropertyType::pulse_width_modulation},
                                {3, ModulatorCommonPropertyType::attack},
                                {8, ModulatorCommonPropertyType::initial_phase_offset},
                                {9, ModulatorCommonPropertyType::attack_time},
                                {10, ModulatorCommonPropertyType::attack_curve},
                                {11, ModulatorCommonPropertyType::decay_time},
                                {12, ModulatorCommonPropertyType::sustain_level},
                                {14, ModulatorCommonPropertyType::release_time},
                                {13, ModulatorCommonPropertyType::sustain_time},
                                {19, ModulatorCommonPropertyType::initial_delay},
                                {16, ModulatorCommonPropertyType::duration},
                                {17, ModulatorCommonPropertyType::loop},
                                {18, ModulatorCommonPropertyType::playback_rate},
                                {1, ModulatorCommonPropertyType::stop_playback}
                            }};

                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                        else
                        {
                            constexpr std::array<std::pair<uint8_t, ModulatorCommonPropertyType>, 20> value_package = {{
                                {0, ModulatorCommonPropertyType::scope},
                                {16, ModulatorCommonPropertyType::trigger_on},
                                {2, ModulatorCommonPropertyType::depth},
                                {4, ModulatorCommonPropertyType::frequency},
                                {5, ModulatorCommonPropertyType::waveform},
                                {6, ModulatorCommonPropertyType::smoothing},
                                {7, ModulatorCommonPropertyType::pulse_width_modulation},
                                {3, ModulatorCommonPropertyType::attack},
                                {8, ModulatorCommonPropertyType::initial_phase_offset},
                                {10, ModulatorCommonPropertyType::attack_time},
                                {11, ModulatorCommonPropertyType::attack_curve},
                                {12, ModulatorCommonPropertyType::decay_time},
                                {13, ModulatorCommonPropertyType::sustain_level},
                                {15, ModulatorCommonPropertyType::release_time},
                                {14, ModulatorCommonPropertyType::sustain_time},
                                {20, ModulatorCommonPropertyType::initial_delay},
                                {17, ModulatorCommonPropertyType::duration},
                                {18, ModulatorCommonPropertyType::loop},
                                {19, ModulatorCommonPropertyType::playback_rate},
                                {1, ModulatorCommonPropertyType::stop_playback}
                            }};

                            //---------------------------------
                            if constexpr (std::is_same<InputType, uint8_t>::value)
                            {
                                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.first == data; });
                                    it != value_package.end())
                                {
                                    return it->second;
                                }
                            }
                            if constexpr (std::is_enum<InputType>::value)
                            {
                                static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                                if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                           { return element.second == data; });
                                    it != value_package.end())
                                {
                                    return it->first;
                                }
                            }
                        }
                    }
                }
            }
            if constexpr (std::is_same<InputType, AudioCommonPropertyType>::value || std::is_same<ReturnType, AudioCommonPropertyType>::value)
            {
                if constexpr (std::is_same<ReturnType, size_t>::value)
                {
                    try_assert(false, "common property type is not return size");
                }
                else
                {
                    if (k_version < 88_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 19> value_package = {{
                            {4, AudioCommonPropertyType::bus_volume},
                            {23, AudioCommonPropertyType::output_bus_volume},
                            {24, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {22, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {18, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {13, AudioCommonPropertyType::positioning_center_percent},
                            {11, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {5, AudioCommonPropertyType::playback_priority_value},
                            {6, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {7, AudioCommonPropertyType::playback_loop},
                            {8, AudioCommonPropertyType::motion_volume_offset},
                            {9, AudioCommonPropertyType::motion_low_pass_filter}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 112_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 28> value_package = {{
                            {4, AudioCommonPropertyType::bus_volume},
                            {23, AudioCommonPropertyType::output_bus_volume},
                            {24, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {33, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {22, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {18, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {13, AudioCommonPropertyType::positioning_center_percent},
                            {11, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {26, AudioCommonPropertyType::hdr_threshold},
                            {27, AudioCommonPropertyType::hdr_ratio},
                            {28, AudioCommonPropertyType::hdr_release_time},
                            {29, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {30, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {31, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {32, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {5, AudioCommonPropertyType::playback_priority_value},
                            {6, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {25, AudioCommonPropertyType::playback_initial_delay},
                            {7, AudioCommonPropertyType::playback_loop},
                            {8, AudioCommonPropertyType::motion_volume_offset},
                            {9, AudioCommonPropertyType::motion_low_pass_filter}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 118_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 43> value_package = {{
                            {5, AudioCommonPropertyType::bus_volume},
                            {23, AudioCommonPropertyType::output_bus_volume},
                            {25, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {24, AudioCommonPropertyType::output_bus_high_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {4, AudioCommonPropertyType::voice_high_pass_filter},
                            {33, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {22, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {18, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {13, AudioCommonPropertyType::positioning_center_percent},
                            {11, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {26, AudioCommonPropertyType::hdr_threshold},
                            {27, AudioCommonPropertyType::hdr_ratio},
                            {28, AudioCommonPropertyType::hdr_release_time},
                            {29, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {30, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {31, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {32, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {45, AudioCommonPropertyType::midi_note_tracking_root_note},
                            {46, AudioCommonPropertyType::midi_event_play_on},
                            {47, AudioCommonPropertyType::midi_transformation_transposition},
                            {48, AudioCommonPropertyType::midi_transformation_velocity_offset},
                            {49, AudioCommonPropertyType::midi_filter_key_range_minimum},
                            {50, AudioCommonPropertyType::midi_filter_key_range_maximum},
                            {51, AudioCommonPropertyType::midi_filter_velocity_minimum},
                            {52, AudioCommonPropertyType::midi_filter_velocity_maximum},
                            {53, AudioCommonPropertyType::midi_filter_channel},
                            {55, AudioCommonPropertyType::midi_clip_tempo_source},
                            {56, AudioCommonPropertyType::midi_target_id},
                            {6, AudioCommonPropertyType::playback_priority_value},
                            {7, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {59, AudioCommonPropertyType::playback_initial_delay},
                            {58, AudioCommonPropertyType::playback_loop},
                            {54, AudioCommonPropertyType::playback_speed},
                            {8, AudioCommonPropertyType::motion_volume_offset},
                            {9, AudioCommonPropertyType::motion_low_pass_filter},
                            {57, AudioCommonPropertyType::mixer_id}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 128_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 43> value_package = {{
                            {5, AudioCommonPropertyType::bus_volume},
                            {24, AudioCommonPropertyType::output_bus_volume},
                            {26, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {25, AudioCommonPropertyType::output_bus_high_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {4, AudioCommonPropertyType::voice_high_pass_filter},
                            {6, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {23, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {22, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {14, AudioCommonPropertyType::positioning_center_percent},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {13, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {27, AudioCommonPropertyType::hdr_threshold},
                            {28, AudioCommonPropertyType::hdr_ratio},
                            {29, AudioCommonPropertyType::hdr_release_time},
                            {30, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {31, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {32, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {33, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {45, AudioCommonPropertyType::midi_note_tracking_root_note},
                            {46, AudioCommonPropertyType::midi_event_play_on},
                            {47, AudioCommonPropertyType::midi_transformation_transposition},
                            {48, AudioCommonPropertyType::midi_transformation_velocity_offset},
                            {49, AudioCommonPropertyType::midi_filter_key_range_minimum},
                            {50, AudioCommonPropertyType::midi_filter_key_range_maximum},
                            {51, AudioCommonPropertyType::midi_filter_velocity_minimum},
                            {52, AudioCommonPropertyType::midi_filter_velocity_maximum},
                            {53, AudioCommonPropertyType::midi_filter_channel},
                            {55, AudioCommonPropertyType::midi_clip_tempo_source},
                            {56, AudioCommonPropertyType::midi_target_id},
                            {7, AudioCommonPropertyType::playback_priority_value},
                            {8, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {59, AudioCommonPropertyType::playback_initial_delay},
                            {58, AudioCommonPropertyType::playback_loop},
                            {54, AudioCommonPropertyType::playback_speed},
                            {9, AudioCommonPropertyType::motion_volume_offset},
                            {10, AudioCommonPropertyType::motion_low_pass_filter},
                            {57, AudioCommonPropertyType::mixer_id}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 132_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 51> value_package = {{
                            {5, AudioCommonPropertyType::bus_volume},
                            {24, AudioCommonPropertyType::output_bus_volume},
                            {26, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {25, AudioCommonPropertyType::output_bus_high_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {4, AudioCommonPropertyType::voice_high_pass_filter},
                            {6, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {23, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {68, AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter},
                            {69, AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {22, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {60, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0},
                            {61, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1},
                            {62, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2},
                            {63, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3},
                            {64, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0},
                            {65, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1},
                            {66, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2},
                            {67, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3},
                            {14, AudioCommonPropertyType::positioning_center_percent},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {13, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {27, AudioCommonPropertyType::hdr_threshold},
                            {28, AudioCommonPropertyType::hdr_ratio},
                            {29, AudioCommonPropertyType::hdr_release_time},
                            {30, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {31, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {32, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {33, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {45, AudioCommonPropertyType::midi_note_tracking_root_note},
                            {46, AudioCommonPropertyType::midi_event_play_on},
                            {47, AudioCommonPropertyType::midi_transformation_transposition},
                            {48, AudioCommonPropertyType::midi_transformation_velocity_offset},
                            {49, AudioCommonPropertyType::midi_filter_key_range_minimum},
                            {50, AudioCommonPropertyType::midi_filter_key_range_maximum},
                            {51, AudioCommonPropertyType::midi_filter_velocity_minimum},
                            {52, AudioCommonPropertyType::midi_filter_velocity_maximum},
                            {53, AudioCommonPropertyType::midi_filter_channel},
                            {55, AudioCommonPropertyType::midi_clip_tempo_source},
                            {56, AudioCommonPropertyType::midi_target_id},
                            {7, AudioCommonPropertyType::playback_priority_value},
                            {8, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {59, AudioCommonPropertyType::playback_initial_delay},
                            {58, AudioCommonPropertyType::playback_loop},
                            {54, AudioCommonPropertyType::playback_speed},
                            {57, AudioCommonPropertyType::mixer_id}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 135_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 53> value_package = {{
                            {5, AudioCommonPropertyType::bus_volume},
                            {24, AudioCommonPropertyType::output_bus_volume},
                            {26, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {25, AudioCommonPropertyType::output_bus_high_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {4, AudioCommonPropertyType::voice_high_pass_filter},
                            {6, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {23, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {68, AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter},
                            {69, AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {22, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {60, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0},
                            {61, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1},
                            {62, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2},
                            {63, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3},
                            {64, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0},
                            {65, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1},
                            {66, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2},
                            {67, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3},
                            {14, AudioCommonPropertyType::positioning_center_percent},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {13, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {71, AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix},
                            {70, AudioCommonPropertyType::positioning_listener_routing_attenuation_id},
                            {27, AudioCommonPropertyType::hdr_threshold},
                            {28, AudioCommonPropertyType::hdr_ratio},
                            {29, AudioCommonPropertyType::hdr_release_time},
                            {30, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {31, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {32, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {33, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {45, AudioCommonPropertyType::midi_note_tracking_root_note},
                            {46, AudioCommonPropertyType::midi_event_play_on},
                            {47, AudioCommonPropertyType::midi_transformation_transposition},
                            {48, AudioCommonPropertyType::midi_transformation_velocity_offset},
                            {49, AudioCommonPropertyType::midi_filter_key_range_minimum},
                            {50, AudioCommonPropertyType::midi_filter_key_range_maximum},
                            {51, AudioCommonPropertyType::midi_filter_velocity_minimum},
                            {52, AudioCommonPropertyType::midi_filter_velocity_maximum},
                            {53, AudioCommonPropertyType::midi_filter_channel},
                            {55, AudioCommonPropertyType::midi_clip_tempo_source},
                            {56, AudioCommonPropertyType::midi_target_id},
                            {7, AudioCommonPropertyType::playback_priority_value},
                            {8, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {59, AudioCommonPropertyType::playback_initial_delay},
                            {58, AudioCommonPropertyType::playback_loop},
                            {54, AudioCommonPropertyType::playback_speed},
                            {57, AudioCommonPropertyType::mixer_id}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 140_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 54> value_package = {{
                            {5, AudioCommonPropertyType::bus_volume},
                            {24, AudioCommonPropertyType::output_bus_volume},
                            {26, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {25, AudioCommonPropertyType::output_bus_high_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {4, AudioCommonPropertyType::voice_high_pass_filter},
                            {6, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {23, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {68, AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter},
                            {69, AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {22, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {60, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0},
                            {61, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1},
                            {62, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2},
                            {63, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3},
                            {64, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0},
                            {65, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1},
                            {66, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2},
                            {72, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3},
                            {67, AudioCommonPropertyType::early_reflection_auxiliary_send_volume},
                            {14, AudioCommonPropertyType::positioning_center_percent},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {13, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {71, AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix},
                            {70, AudioCommonPropertyType::positioning_listener_routing_attenuation_id},
                            {27, AudioCommonPropertyType::hdr_threshold},
                            {28, AudioCommonPropertyType::hdr_ratio},
                            {29, AudioCommonPropertyType::hdr_release_time},
                            {30, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {31, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {32, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {33, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {45, AudioCommonPropertyType::midi_note_tracking_root_note},
                            {46, AudioCommonPropertyType::midi_event_play_on},
                            {47, AudioCommonPropertyType::midi_transformation_transposition},
                            {48, AudioCommonPropertyType::midi_transformation_velocity_offset},
                            {49, AudioCommonPropertyType::midi_filter_key_range_minimum},
                            {50, AudioCommonPropertyType::midi_filter_key_range_maximum},
                            {51, AudioCommonPropertyType::midi_filter_velocity_minimum},
                            {52, AudioCommonPropertyType::midi_filter_velocity_maximum},
                            {53, AudioCommonPropertyType::midi_filter_channel},
                            {55, AudioCommonPropertyType::midi_clip_tempo_source},
                            {56, AudioCommonPropertyType::midi_target_id},
                            {7, AudioCommonPropertyType::playback_priority_value},
                            {8, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {59, AudioCommonPropertyType::playback_initial_delay},
                            {58, AudioCommonPropertyType::playback_loop},
                            {54, AudioCommonPropertyType::playback_speed},
                            {57, AudioCommonPropertyType::mixer_id}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else if (k_version < 150_ui)
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 55> value_package = {{
                            {5, AudioCommonPropertyType::bus_volume},
                            {24, AudioCommonPropertyType::output_bus_volume},
                            {26, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {25, AudioCommonPropertyType::output_bus_high_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {2, AudioCommonPropertyType::voice_pitch},
                            {3, AudioCommonPropertyType::voice_low_pass_filter},
                            {4, AudioCommonPropertyType::voice_high_pass_filter},
                            {6, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {23, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {68, AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter},
                            {69, AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {22, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {60, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0},
                            {61, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1},
                            {62, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2},
                            {63, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3},
                            {64, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0},
                            {65, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1},
                            {66, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2},
                            {72, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3},
                            {67, AudioCommonPropertyType::early_reflection_auxiliary_send_volume},
                            {14, AudioCommonPropertyType::positioning_center_percent},
                            {12, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {13, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {73, AudioCommonPropertyType::positioning_speaker_panning_z},
                            {71, AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix},
                            {70, AudioCommonPropertyType::positioning_listener_routing_attenuation_id},
                            {27, AudioCommonPropertyType::hdr_threshold},
                            {28, AudioCommonPropertyType::hdr_ratio},
                            {29, AudioCommonPropertyType::hdr_release_time},
                            {30, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {31, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {32, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {33, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {45, AudioCommonPropertyType::midi_note_tracking_root_note},
                            {46, AudioCommonPropertyType::midi_event_play_on},
                            {47, AudioCommonPropertyType::midi_transformation_transposition},
                            {48, AudioCommonPropertyType::midi_transformation_velocity_offset},
                            {49, AudioCommonPropertyType::midi_filter_key_range_minimum},
                            {50, AudioCommonPropertyType::midi_filter_key_range_maximum},
                            {51, AudioCommonPropertyType::midi_filter_velocity_minimum},
                            {52, AudioCommonPropertyType::midi_filter_velocity_maximum},
                            {53, AudioCommonPropertyType::midi_filter_channel},
                            {55, AudioCommonPropertyType::midi_clip_tempo_source},
                            {56, AudioCommonPropertyType::midi_target_id},
                            {7, AudioCommonPropertyType::playback_priority_value},
                            {8, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {59, AudioCommonPropertyType::playback_initial_delay},
                            {58, AudioCommonPropertyType::playback_loop},
                            {54, AudioCommonPropertyType::playback_speed},
                            {57, AudioCommonPropertyType::mixer_id}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                    else
                    {
                        constexpr std::array<std::pair<uint8_t, AudioCommonPropertyType>, 54> value_package = {{
                            {4, AudioCommonPropertyType::bus_volume},
                            {13, AudioCommonPropertyType::output_bus_volume},
                            {15, AudioCommonPropertyType::output_bus_low_pass_filter},
                            {14, AudioCommonPropertyType::output_bus_high_pass_filter},
                            {0, AudioCommonPropertyType::voice_volume},
                            {1, AudioCommonPropertyType::voice_pitch},
                            {2, AudioCommonPropertyType::voice_low_pass_filter},
                            {3, AudioCommonPropertyType::voice_high_pass_filter},
                            {5, AudioCommonPropertyType::voice_volume_make_up_gain},
                            {12, AudioCommonPropertyType::game_defined_auxiliary_send_volume},
                            {24, AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter},
                            {25, AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter},
                            {8, AudioCommonPropertyType::user_defined_auxiliary_send_volume_0},
                            {9, AudioCommonPropertyType::user_defined_auxiliary_send_volume_1},
                            {10, AudioCommonPropertyType::user_defined_auxiliary_send_volume_2},
                            {11, AudioCommonPropertyType::user_defined_auxiliary_send_volume_3},
                            {16, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0},
                            {17, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1},
                            {18, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2},
                            {19, AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3},
                            {20, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0},
                            {21, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1},
                            {22, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2},
                            {23, AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3},
                            {26, AudioCommonPropertyType::early_reflection_auxiliary_send_volume},
                            {41, AudioCommonPropertyType::positioning_center_percent},
                            {35, AudioCommonPropertyType::positioning_speaker_panning_x},
                            {36, AudioCommonPropertyType::positioning_speaker_panning_y},
                            {37, AudioCommonPropertyType::positioning_speaker_panning_z},
                            {42, AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix},
                            {85, AudioCommonPropertyType::positioning_listener_routing_attenuation_id},
                            {27, AudioCommonPropertyType::hdr_threshold},
                            {28, AudioCommonPropertyType::hdr_ratio},
                            {29, AudioCommonPropertyType::hdr_release_time},
                            {61, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id},
                            {62, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum},
                            {63, AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum},
                            {30, AudioCommonPropertyType::hdr_envelope_tracking_active_range},
                            {75, AudioCommonPropertyType::midi_note_tracking_root_note},
                            {76, AudioCommonPropertyType::midi_event_play_on},
                            {31, AudioCommonPropertyType::midi_transformation_transposition},
                            {32, AudioCommonPropertyType::midi_transformation_velocity_offset},
                            {77, AudioCommonPropertyType::midi_filter_key_range_minimum},
                            {78, AudioCommonPropertyType::midi_filter_key_range_maximum},
                            {79, AudioCommonPropertyType::midi_filter_velocity_minimum},
                            {80, AudioCommonPropertyType::midi_filter_velocity_maximum},
                            {81, AudioCommonPropertyType::midi_filter_channel},
                            {82, AudioCommonPropertyType::midi_clip_tempo_source},
                            {83, AudioCommonPropertyType::midi_target_id},
                            {6, AudioCommonPropertyType::playback_priority_value},
                            {56, AudioCommonPropertyType::playback_priority_offset_at_maximum_distance},
                            {34, AudioCommonPropertyType::playback_initial_delay},
                            {84, AudioCommonPropertyType::playback_loop},
                            {33, AudioCommonPropertyType::playback_speed}
                        }};

                        //---------------------------------
                        if constexpr (std::is_same<InputType, uint8_t>::value)
                        {
                            static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.first == data; });
                                it != value_package.end())
                            {
                                return it->second;
                            }
                        }
                        if constexpr (std::is_enum<InputType>::value)
                        {
                            static_assert(std::is_same<ReturnType, uint8_t>::value, "Return type must be uint8_t");
                            if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                       { return element.second == data; });
                                it != value_package.end())
                            {
                                return it->first;
                            }
                        }
                    }
                }
            }

            //------------------------------
            if constexpr (std::is_same<ReturnType, size_t>::value)
            {
                return k_none_size;
            }
            if constexpr (std::is_same<InputType, uint8_t>::value)
            {
                static_assert(std::is_enum<ReturnType>::value, "Return type must be enumeration type");
                auto type = std::string_view{std::to_string(data)};
                assert_conditional(false, String::format(fmt::format("{}", Language::get("miscellaneous.shared.failed_cast_enum_index")), type, magic_enum::enum_type_name<ReturnType>()), "exchange_enumeration");
            }
        }

        template <typename Type>
            requires(std::is_enum<Type>::value)
        inline static auto declare_common_property_type(
            Type const &type) -> std::any
        {
            if constexpr (std::is_same<Type, EventActionCommonPropertyType>::value)
            {
                if (k_version < 118_ui)
                {
                    auto value_package = std::array<std::pair<EventActionCommonPropertyType, std::any>, 3>{
                        std::pair(EventActionCommonPropertyType::delay, std::make_any<uint32_t>(0_ui)),
                        std::pair(EventActionCommonPropertyType::fade_time, std::make_any<uint32_t>(0_ui)),
                        std::pair(EventActionCommonPropertyType::probability, std::make_any<float>(100.0f))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else if (k_version < 150_ui)
                {
                    auto value_package = std::array<std::pair<EventActionCommonPropertyType, std::any>, 3>{
                        std::pair(EventActionCommonPropertyType::delay, std::make_any<uint32_t>(0_ui)),
                        std::pair(EventActionCommonPropertyType::fade_time, std::make_any<uint32_t>(0_ui)),
                        std::pair(EventActionCommonPropertyType::probability, std::make_any<float>(100.0f))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else
                {
                    auto value_package = std::array<std::pair<EventActionCommonPropertyType, std::any>, 3>{
                        std::pair(EventActionCommonPropertyType::delay, std::make_any<uint32_t>(0_ui)),
                        std::pair(EventActionCommonPropertyType::fade_time, std::make_any<uint32_t>(0_ui)),
                        std::pair(EventActionCommonPropertyType::probability, std::make_any<float>(100.0f))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
            }
            if constexpr (std::is_same<Type, ModulatorCommonPropertyType>::value)
            {
                if (k_version >= 112_ui)
                {
                    if (k_version < 150_ui)
                    {
                        auto value_package = std::array<std::pair<ModulatorCommonPropertyType, std::any>, 20>{
                            std::pair(ModulatorCommonPropertyType::scope, std::make_any<uint32_t>(0_ui)),
                            std::pair(ModulatorCommonPropertyType::trigger_on, std::make_any<uint32_t>(0_ui)),
                            std::pair(ModulatorCommonPropertyType::depth, std::make_any<float>(100.0f)),
                            std::pair(ModulatorCommonPropertyType::frequency, std::make_any<float>(1.0f)),
                            std::pair(ModulatorCommonPropertyType::waveform, std::make_any<uint32_t>(0_ui)),
                            std::pair(ModulatorCommonPropertyType::smoothing, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::pulse_width_modulation, std::make_any<float>(50.0f)),
                            std::pair(ModulatorCommonPropertyType::attack, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::initial_phase_offset, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::attack_time, std::make_any<float>(0.2f)),
                            std::pair(ModulatorCommonPropertyType::attack_curve, std::make_any<float>(50.0f)),
                            std::pair(ModulatorCommonPropertyType::decay_time, std::make_any<float>(0.2f)),
                            std::pair(ModulatorCommonPropertyType::sustain_level, std::make_any<float>(100.0f)),
                            std::pair(ModulatorCommonPropertyType::release_time, std::make_any<float>(0.5f)),
                            std::pair(ModulatorCommonPropertyType::sustain_time, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::initial_delay, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::duration, std::make_any<float>(1.0f)),
                            std::pair(ModulatorCommonPropertyType::loop, std::make_any<uint32_t>(1_ui)),
                            std::pair(ModulatorCommonPropertyType::playback_rate, std::make_any<float>(1.0f)),
                            std::pair(ModulatorCommonPropertyType::stop_playback, std::make_any<bool>(true))};

                        //---------------------------------
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == type; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                    else
                    {
                        auto value_package = std::array<std::pair<ModulatorCommonPropertyType, std::any>, 20>{
                            std::pair(ModulatorCommonPropertyType::scope, std::make_any<uint32_t>(0_ui)),
                            std::pair(ModulatorCommonPropertyType::trigger_on, std::make_any<uint32_t>(0_ui)),
                            std::pair(ModulatorCommonPropertyType::depth, std::make_any<float>(100.0f)),
                            std::pair(ModulatorCommonPropertyType::frequency, std::make_any<float>(1.0f)),
                            std::pair(ModulatorCommonPropertyType::waveform, std::make_any<uint32_t>(0_ui)),
                            std::pair(ModulatorCommonPropertyType::smoothing, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::pulse_width_modulation, std::make_any<float>(50.0f)),
                            std::pair(ModulatorCommonPropertyType::attack, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::initial_phase_offset, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::attack_time, std::make_any<float>(0.2f)),
                            std::pair(ModulatorCommonPropertyType::attack_curve, std::make_any<float>(50.0f)),
                            std::pair(ModulatorCommonPropertyType::decay_time, std::make_any<float>(0.2f)),
                            std::pair(ModulatorCommonPropertyType::sustain_level, std::make_any<float>(100.0f)),
                            std::pair(ModulatorCommonPropertyType::release_time, std::make_any<float>(0.5f)),
                            std::pair(ModulatorCommonPropertyType::sustain_time, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::initial_delay, std::make_any<float>(0.0f)),
                            std::pair(ModulatorCommonPropertyType::duration, std::make_any<float>(1.0f)),
                            std::pair(ModulatorCommonPropertyType::loop, std::make_any<uint32_t>(1_ui)),
                            std::pair(ModulatorCommonPropertyType::playback_rate, std::make_any<float>(1.0f)),
                            std::pair(ModulatorCommonPropertyType::stop_playback, std::make_any<bool>(true))};

                        //---------------------------------
                        if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                                   { return element.first == type; });
                            it != value_package.end())
                        {
                            return it->second;
                        }
                    }
                }
            }
            if constexpr (std::is_same<Type, AudioCommonPropertyType>::value)
            {
                if (k_version < 88_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 19>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::motion_volume_offset, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::motion_low_pass_filter, std::make_any<float>(0.0f))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else if (k_version < 112_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 28>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::motion_volume_offset, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::motion_low_pass_filter, std::make_any<float>(0.0f))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }

                else if (k_version < 118_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 43>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::midi_note_tracking_root_note, std::make_any<uint32_t>(60_ui)),
                        std::pair(AudioCommonPropertyType::midi_event_play_on, std::make_any<uint32_t>(0_ui)), // TODO.
                        std::pair(AudioCommonPropertyType::midi_transformation_transposition, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_velocity_offset, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_channel, std::make_any<uint32_t>(65535_ui)),
                        std::pair(AudioCommonPropertyType::midi_clip_tempo_source, std::make_any<uint32_t>(0_ui)), // TODO.
                        std::pair(AudioCommonPropertyType::midi_target_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_speed, std::make_any<float>(1.0f)),
                        std::pair(AudioCommonPropertyType::motion_volume_offset, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::motion_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::mixer_id, std::make_any<uint32_t>(0_ui))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else if (k_version < 128_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 43>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::midi_note_tracking_root_note, std::make_any<uint32_t>(60_ui)),
                        std::pair(AudioCommonPropertyType::midi_event_play_on, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_transposition, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_velocity_offset, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_channel, std::make_any<uint32_t>(65535_ui)),
                        std::pair(AudioCommonPropertyType::midi_clip_tempo_source, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_target_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_speed, std::make_any<float>(1.0f)),
                        std::pair(AudioCommonPropertyType::motion_volume_offset, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::motion_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::mixer_id, std::make_any<uint32_t>(0_ui))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else if (k_version < 132_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 51>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::midi_note_tracking_root_note, std::make_any<uint32_t>(60_ui)),
                        std::pair(AudioCommonPropertyType::midi_event_play_on, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_transposition, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_velocity_offset, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_channel, std::make_any<uint32_t>(65535_ui)),
                        std::pair(AudioCommonPropertyType::midi_clip_tempo_source, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_target_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_speed, std::make_any<float>(1.0f)),
                        std::pair(AudioCommonPropertyType::mixer_id, std::make_any<uint32_t>(0_ui))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else if (k_version < 135_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 53>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix, std::make_any<float>(100.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_attenuation_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::midi_note_tracking_root_note, std::make_any<uint32_t>(60_ui)),
                        std::pair(AudioCommonPropertyType::midi_event_play_on, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_transposition, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_velocity_offset, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_channel, std::make_any<uint32_t>(65535_ui)),
                        std::pair(AudioCommonPropertyType::midi_clip_tempo_source, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_target_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_speed, std::make_any<float>(1.0f)),
                        std::pair(AudioCommonPropertyType::mixer_id, std::make_any<uint32_t>(0_ui))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else if (k_version < 140_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 54>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::early_reflection_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix, std::make_any<float>(100.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_attenuation_id, 0_ui),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::midi_note_tracking_root_note, std::make_any<uint32_t>(60_ui)),
                        std::pair(AudioCommonPropertyType::midi_event_play_on, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_transposition, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_velocity_offset, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_channel, std::make_any<uint32_t>(65535_ui)),
                        std::pair(AudioCommonPropertyType::midi_clip_tempo_source, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_target_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_speed, std::make_any<float>(1.0f)),
                        std::pair(AudioCommonPropertyType::mixer_id, std::make_any<uint32_t>(0_ui))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else if (k_version < 150_ui)
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 55>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::early_reflection_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_z, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix, std::make_any<float>(100.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_attenuation_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::midi_note_tracking_root_note, std::make_any<uint32_t>(60_ui)),
                        std::pair(AudioCommonPropertyType::midi_event_play_on, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_transposition, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_velocity_offset, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_maximum, std::make_any<uint32_t>(127_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_channel, std::make_any<uint32_t>(65535_ui)),
                        std::pair(AudioCommonPropertyType::midi_clip_tempo_source, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_target_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_speed, std::make_any<float>(1.0f)),
                        std::pair(AudioCommonPropertyType::mixer_id, std::make_any<uint32_t>(0_ui))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
                else
                {
                    auto value_package = std::array<std::pair<AudioCommonPropertyType, std::any>, 54>{
                        std::pair(AudioCommonPropertyType::bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::output_bus_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_pitch, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::voice_volume_make_up_gain, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_low_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::game_defined_auxiliary_send_high_pass_filter, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_volume_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_low_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_0, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_1, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_2, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::user_defined_auxiliary_send_high_pass_filter_3, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::early_reflection_auxiliary_send_volume, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_center_percent, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_x, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_y, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_speaker_panning_z, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_speaker_panning_division_spatialization_mix, std::make_any<float>(100.0f)),
                        std::pair(AudioCommonPropertyType::positioning_listener_routing_attenuation_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_threshold, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_ratio, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_release_time, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_minimum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_window_tap_output_game_parameter_maximum, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::hdr_envelope_tracking_active_range, std::make_any<float>(12.0f)),
                        std::pair(AudioCommonPropertyType::midi_note_tracking_root_note, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::midi_event_play_on, std::make_any<uint32_t>(60_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_transposition, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_transformation_velocity_offset, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_key_range_maximum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_minimum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_velocity_maximum, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_filter_channel, std::make_any<uint32_t>(65535_ui)),
                        std::pair(AudioCommonPropertyType::midi_clip_tempo_source, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::midi_target_id, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_priority_value, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_priority_offset_at_maximum_distance, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_initial_delay, std::make_any<float>(0.0f)),
                        std::pair(AudioCommonPropertyType::playback_loop, std::make_any<uint32_t>(0_ui)),
                        std::pair(AudioCommonPropertyType::playback_speed, std::make_any<float>(1.0f))};

                    //---------------------------------
                    if (auto it = std::find_if(value_package.begin(), value_package.end(), [&](auto &element)
                                               { return element.first == type; });
                        it != value_package.end())
                    {
                        return it->second;
                    }
                }
            }
        }
    };
}