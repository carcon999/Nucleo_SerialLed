/*
  WS2812 LED Control Sample v1.00  2014/05/31
  Created by @carcon999

  実行には、Serial LEDシールドが必要です。
  http://blogs.yahoo.co.jp/carcon999/37942815.html

  シールドのジャンパー設定は以下の設定としてください。
  上段  □■■    ■■    □×□  上段
  下段  □■■    ■■    □■■  下段
        JP3      JP2      JP1

  JP1の上段は中央のPIN（上記の×）をmbedのD10にジャンパーケーブルで接続する
  必要があります。

 */
// include library codes
#include "mbed.h"
#include "SerialLedController.h"

/* Commands Table. Run sequentially. */
const uint8_t led_play_list[][SERIALCOMMAND_FIXEDLEN] =
{
  // 以下の演出テーブルを変更することで別な演出に切り替えることが可能です。
  { 0x94, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* {"name":"script","ctrl":148,"mode":1} */
  { 0x90, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00 }, /* {"name":"system","ctrl":144,"length":32,"wait":0} */
  { 0x95, 0x40, 0x00, 0x05, 0xf4, 0x01, 0x00, 0x00 }, /* {"name":"rainbow","ctrl":149,"bright":64,"mode":0,"wait":5,"loop":500} */
  { 0x97, 0x40, 0x03, 0x0a, 0x04, 0x00, 0x00, 0x00 }, /* {"name":"seesaw","ctrl":151,"bright":64,"loop":3,"wait":10,"rgbflag":4} */
  { 0x92, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0a, 0x00 }, /* {"name":"shift","ctrl":146,"dir":0,"mode":0,"count":32,"wait":10} */
  { 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* {"name":"script","ctrl":148,"mode":0} */
};

// シリアルのインスタンスを生成する。
// TXとRXのPIN番号を指定する。
Serial sr(D10, D2);

// シリアルコントローラ用ライブラリのインスタンスを生成する。
// リセットとボーレート指定用のPIN番号を指定する。もし利用しない場合は－１を指定する。
SerialLedController controller(D4, D5);

void setup()
{
  // シリアルクラスと、通信速度（true:57600, false:19200）を指定する
  controller.begin(&sr, true);

  // 演出用のテーブルを与える。（コントローラにデータを転送する）  
  if(controller.write(led_play_list, sizeof(led_play_list))){
    // 転送に成功したら演出を開始する。
    controller.start();
  }
}

void loop()
{
}

int main() 
{
  setup();

  while(1) 
  {
      loop();
  }
}
