export QT_HOME=/opt/Qt-5.12.12                           
export QT_QPA_FB_DRM=1                                 
export QT_QPA_GENERIC_PLUGINS=evdevkeyboard            
export QT_QPA_GENERIC_PLUGINS=evdevmouse               
export QT_QPA_EVDEV_MOUSE_PARAMETERS=/dev/input/event0    
export QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/event1 
export QT_QPA_FONTDIR=$QT_HOME/lib/fonts                  
export QT_PLUGIN_PATH=$QT_HOME/plugins                    
export QT_QPA_PLATFORM=linuxfb:fb=/dev/fb0                
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$QT_HOME/lib"    
./aes_qt.ke
