#include <fluidsynth.h>
#include <iostream>
#include <string>
#include <limits>

unsigned int buffer_size = 64;
float volume = 0.5;
float sample_rate = 48000;
bool exit_program =false;
bool change_status =true;

 // Deklarasi variabel untuk load soundfont gamelan//
enum ch_instrumen {BONANG=0, DEMUNG=1};
const std::string sfpath = "./sf2list/";
const std::string sfsuffix = ".sf2";
const std::string instrumen[2]={"Bonang","Demung"};
fluid_settings_t* settings;
fluid_synth_t* synth;
fluid_midi_driver_t* mdriver;
fluid_audio_driver_t* adriver;

void Clear(void){
    #if _WIN32
        std::system("cls");
    #elif __linux__
        std::system("clear");
    #endif
}

void load_alsa(fluid_settings_t *settings){
    #if _WIN32
        fluid_settings_setstr(settings,"midi.driver", "winmidi");
        fluid_settings_setstr(settings,"audio.driver", "dsound");
    #elif __linux__
        fluid_settings_setstr(settings,"midi.driver", "alsa_seq");
        fluid_settings_setint(settings,"midi.realtime-prio",80);
        fluid_settings_setstr(settings,"audio.driver", "alsa");
        fluid_settings_setint(settings,"audio.realtime-prio",80);
        fluid_settings_setint(settings,"midi.autoconnect",1);
    #endif
}

int handle_midi_event(void* data, fluid_midi_event_t* event){
    fluid_synth_t* synth = (fluid_synth_t*) data;
    int type = fluid_midi_event_get_type(event);
    int key = fluid_midi_event_get_key(event);
    int vel = fluid_midi_event_get_velocity(event);
    int chan = fluid_midi_event_get_channel(event);
    switch(type)
    {
    case 144:
        return fluid_synth_noteon(synth, chan,
                                  key,
                                  fluid_midi_event_get_velocity(event));

    case 128:
        return fluid_synth_noteoff(synth, chan, key);
    default:
        return 0;
    }
    return -1;

}

void load_sf2(fluid_synth_t* synth){
    for(int i=0; i < 2 ;i++){
        std::string path = sfpath+instrumen[i]+sfsuffix;
        fluid_synth_sfload(synth, path.c_str() ,0);
        fluid_synth_program_select_by_sfont_name(synth,i,path.c_str(),0,0);
    }
}

void init_fluidsynth(void){
    fluid_set_log_function(FLUID_WARN, NULL, NULL);
    fluid_set_log_function(FLUID_INFO, NULL, NULL);
    settings = new_fluid_settings();
    load_alsa(settings);
    synth = new_fluid_synth(settings);
    fluid_settings_setint(settings, "synth.min-note-length", 0);
    fluid_settings_setint(settings, "synth.cpu-cores", 2);
    fluid_settings_setint(settings, "synth.polyphony", 32);
    fluid_settings_setint(settings, "audio.period-size", buffer_size);
    fluid_settings_setint(settings, "audio.periods", 16);
    fluid_settings_setnum(settings, "synth.sample-rate", sample_rate);
    fluid_settings_setnum(settings, "synth.gain", volume);
    fluid_settings_setnum(settings, "synth.overflow.released", 500);
    fluid_settings_setnum(settings, "synth.overflow.sustained", 1000);
    load_sf2(synth);
    mdriver = new_fluid_midi_driver(settings, handle_midi_event, synth);
    adriver = new_fluid_audio_driver(settings, synth);
}

void delete_fluidsynth(void){
    delete_fluid_midi_driver(mdriver);
    delete_fluid_synth(synth);
    delete_fluid_audio_driver(adriver);
    delete_fluid_settings(settings);
}

void menu(void){
    int choice;
    while(!exit_program){
        Clear();
        init_fluidsynth();
        std::cout << "Gameltron UGM\n\n";
        std::cout << "1. Atur Volume\n";
        std::cout << "\n2. Atur Sample Rate\n";
        std::cout << "\n3. Atur Buffer Size\n";
        std::cout << "\n0. Keluar\n";
        std::cout << "\nPilihan Anda : ";
        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        switch (choice) {
            case 1:
                std::cout << "\nUbah nilai volume (0-10) : ";
                std::cin >> volume;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                delete_fluidsynth();
                break;
            case 2:
                std::cout << "\nUbah nilai sample rate (8000 - 96000) : ";
                std::cin >> sample_rate;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                delete_fluidsynth();
                break;
            case 3:
                std::cout << "\nUbah nilai buffer size (64 - 8192) : ";
                std::cin >> buffer_size;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                delete_fluidsynth();
                break;
            case 0:
                exit_program=true;
                delete_fluidsynth();
                break;
            default:
                std::cout << "Opsi tidak tersedia, silahkan pilih kembali" << std::endl;
                break;
        }
    }
    Clear();
}

int main(int argc,char **argv) 
{
    menu();
    return 0;
}