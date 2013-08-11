msg[10];
int val; 
// Te set brightness
msg[0] = 0x53;
msg[1] = 0x00;
msg[2] = 0x8A; 
msg[3] = 0x00; 
msg[4] = 0x00; 

msg[5] = (int)(val/256); 
msg[6] = val%256;
val = msg[1] +msg[2] +msg[3] +msg[4] +msg[5] +msg[6];
val = val % 0xFF;
msg[7] = val;
msg[8] = 0x45;
msg[9] = '\r';
	
Write(msg,10);

// To set contrast 
msg[2] = 0x85;

// To save settings
msg[2] = 0x5B;

//To restore factory defaults
msg[2] = 0x5A;

// To set polarity 
msg[2] = 0x0A;
msg[3] = 0x00; // For white hot
msg[3] = 0x01; // for black hot
msg[3] = 0x02; // for Iron
msg[3] = 0x03; // for Rainbow
msg[3] = 0x04; // for Green-red 
