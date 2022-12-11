#ifndef SHOWTEXT_H
#define SHOWTEXT_H
#include "Arduino.h"

namespace ALIGN {
	enum{LEFT, CENTER, RIGHT};
};

class ShowText {
	
		typedef struct {
			bool enable = 0, sw = 0;
			uint16_t speed = 0;
			uint32_t update = 0;
		}TBLINK;
	
	private:
		bool scroll = false, shown = false, spaces_on_start = false, spaces_on_end = false;
		uint8_t max_length = 0, text_align = 0, begin_position = 0;
		uint8_t delay_on_start = 0, delay_on_end = 0, dlst = 0, dled = 0;
		uint16_t text_length = 0, scroll_speed = 0, text_position = 0;
		uint32_t update = 0, update2 = 0;
		TBLINK blink;
		String text_buff = "", text = "", space = "";
		
		void(*end_scroll_callback)( void ) = NULL;
		void(*show_callback)( const char* ) = NULL;
		
		String hide( void ){
			String spaces = "";
			uint8_t len = this->text_align == ALIGN::LEFT ? this->text_length : this->max_length;
								
				for( uint8_t i=0; i<len; i++ ){
					spaces += " ";
				}
				
			return spaces;
		}

		String get( void ){
			String text = "";
			this->scroll = false;
			
				if( this->text_length <= this->max_length ){
				
						switch( this->text_align ){
							case ALIGN::LEFT:{
								text = this->text_buff;
							}break;
							case ALIGN::CENTER:{
								uint8_t spbg = (this->max_length-(uint8_t)this->text_length)/2;
								
									for( uint8_t i=0; i<spbg; i++ ){
										text += this->space;
									}
									
								text = text+this->text_buff;
								uint8_t spend = this->max_length-(uint8_t)text.length();
								
									for( uint8_t i=0; i<spend; i++ ){
										text += this->space;
									}
							}break;
							case ALIGN::RIGHT:{
								
									for( uint8_t i=0; i<(this->max_length-this->text_length); i++ ){
										text += this->space;
									}
									
								text = text+this->text_buff;
							}break;
						}
				} else {
					this->scroll = true;		
					text = this->text_buff;
					
						if( this->spaces_on_start || this->spaces_on_end ){
							String spaces = "";
								
								for( uint8_t i=0; i<this->max_length; i++ ){
									spaces += this->space;
								}
								
								if( this->spaces_on_start ) text = spaces+text;
								
								if( this->spaces_on_end ) text = text+spaces;
						}
						
					uint16_t len = text.length()-this->max_length;
					
						if( this->text_position < len ){
							this->dled = this->delay_on_end;							
								
								if( this->dlst ) this->dlst--;
								else this->text_position++;
						
						} else {
							this->dlst = this->delay_on_start;
							
								if( this->dled ) this->dled--;
								else {
									
										if( this->end_scroll_callback ){
											this->end_scroll_callback();
										}
									
									this->text_position = 0;
									this->scroll = false;
								}
						}

					return text.substring( this->text_position, this->text_position+this->max_length );
				}	
		
			return text;
		}		

	public:
		void begin( uint8_t max_length, uint8_t begin_position=0 ){
			this->max_length = max_length;
			this->begin_position = begin_position;
		}
		
		void registerShowCallback( void(*callback)( const char* ) ){
			this->show_callback = callback;
		}
		
		void registerEndScrollCallback( void(*callback)( void ) ){
			this->end_scroll_callback = callback;
		}
		
		uint8_t getX( void ){
			return this->begin_position;
		}
		
		void setDelayOnStart( uint8_t dl=3 ){
			this->delay_on_start = dl;
			this->dlst = this->delay_on_start;
		}
		
		void setDelayOnEnd( uint8_t dl=3 ){
			this->delay_on_end = dl;
			this->dled = this->delay_on_end;
		}
		
		void setScrollSpeed( uint16_t speed=500 ){
			this->scroll_speed = speed;
		}
		
		void setSpace( const char* space=" " ){
			this->space = String( space );
		}
		
		void setSpacesOnStart( bool spaces=false ){
			this->spaces_on_start = spaces;
		}
		
		void setSpacesOnEnd( bool spaces=false ){
			this->spaces_on_end = spaces;
		}
		
		void setMaxLength( uint8_t length ){
			this->max_length = length;
			this->max_length = this->max_length-this->begin_position;
		}
		
		void setBeginPosition( uint8_t begin_position ){
			this->begin_position = begin_position;
			this->max_length = this->max_length-this->begin_position;
		}
		
		void setBlink( bool enable=false, uint16_t speed=500 ){
			this->blink.enable = enable;
			this->blink.speed = speed;
			this->shown = this->blink.enable;
		}
		
		void set( const char* text, uint8_t align=ALIGN::CENTER ){
		
				if( String( text ) != this->text ){
					this->text = text;
					this->text_align = align;
					this->text_buff = this->text;
					this->text_buff.replace( "ą", "a" );
					this->text_buff.replace( "ć", "c" );
					this->text_buff.replace( "ę", "e" );
					this->text_buff.replace( "ł", "l" );
					this->text_buff.replace( "ń", "n" );
					this->text_buff.replace( "ó", "o" );
					this->text_buff.replace( "ś", "s" );
					this->text_buff.replace( "ż", "z" );
					this->text_buff.replace( "ź", "z" );
					this->text_buff.replace( "Ą", "A" );
					this->text_buff.replace( "Ć", "C" );
					this->text_buff.replace( "Ę", "E" );
					this->text_buff.replace( "Ł", "L" );
					this->text_buff.replace( "Ń", "N" );
					this->text_buff.replace( "Ó", "O" );
					this->text_buff.replace( "Ś", "S" );
					this->text_buff.replace( "Ż", "Z" );
					this->text_buff.replace( "Ź", "Z" );
					this->text_length = this->text_buff.length();
					this->text_position = 0;
					
					this->setBeginPosition( this->begin_position );
					this->setSpacesOnStart();
					this->setSpacesOnEnd();
					this->setSpace();
					this->setScrollSpeed();
					this->setDelayOnStart();
					this->setDelayOnEnd();
					this->setBlink();
					this->shown = false;
				}
		}
		
		void show( void ){
				
				if( this->show_callback == NULL ) return;
				
			uint32_t curr = millis();
			
				if( this->scroll ){
			
						if( curr-this->update>=this->scroll_speed ){
							this->update = curr;
							this->show_callback( this->get().c_str() );
						}
						
				} else if( !this->shown ){
					this->shown = true;
					this->show_callback( this->get().c_str() );
				} else if( this->text_length > this->max_length ) this->scroll = true;
				
				if( this->blink.enable && !this->scroll ){
						
						if( curr-this->blink.update>=this->blink.speed ){
							this->blink.update = curr;
							this->blink.sw ^= 1;
							
								if( this->blink.sw ){
									this->show_callback( this->get().c_str() );
								} else {
									this->show_callback( this->hide().c_str() );
								}
						}
						
				}
		}
		
		String show( const char* str, uint8_t align=ALIGN::CENTER, const char* space=" " ){
			this->set( str, align );
			this->setSpace( space );
			this->shown = true;
			return this->get();
		}
		
		bool isScrolling( void ){
			return (this->text_length > this->max_length) ? true : false;
		}
};
#endif