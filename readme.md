# LabelAttributedBMFont

## 概要

一文字ずつ表示していくラベル  
タップされた場合は、  
複数ページあると次のページの文章を表示するようなページ送り機能つき

強調キーワードの設定も可能(複数可)

大元は[コチラ](http://survive-engineer.hatenablog.com/entry/2014/02/13/013103)
これを元にcocos2d-x v3.0用に書き換え、自分用に幾つか機能の追加＆削除

お気に召したら是非使ってみて(テストしてｗ)ください〜  
そして何かアレば、コメントなり[@AnzNetJp](https://twitter.com/AnzNetJp)にリプなりくださいな

## 簡単に使い方

    // 表示文字準備
    vector< string > pages;
    pages.push_back("あいうえおー\nかきくけこー");
    pages.push_back("さしすせそー");
    
    auto label = LabelAttributedBMFont::createWithBMFont("hoge.fnt", pages);
    
    /*
    普通に文字列でもok
    auto label = LabelAttributedBMFont::createWithBMFont("hoge.fnt", "ページ送りはないよー\n1ページだけだよー");
    */
    
    // 強調キーワードを設定する場合
    label->addKeyWord("おー", Color3B::RED);   // 強調キーワード, 強調色
    // 複数あるならもう繰り返し
    label->addKeyWord("すせー", Color3B::YELLOW);   // 強調キーワード, 強調色
	
    // コールバック設定するなら(全表示が終わったときにタップされると呼ばれる)
    label->setCallback([](Object *sender){
      // 処理記述
    });
	
	// コールバック設定その２(ページ送りするたびに呼ばれる)
	// もどってくる値はpagesのindex
	label->setCallbackChangedPage([](int index) {
		// indexから現在何が表示されているのか判定して何か処理
		// ページ送りされたらなので、indexは1から(2ページ目からしかこない)
	});
    
	// 文字送りしない場合(ページ送りとキーワード強調は有効にしたい場合)
	// label->setDispSpeed(0);
	
    // 文字送り開始(これやんないと何も始まらないYo)
    label->start();

## リリースノート

2014.06.28
:	cocos2d-x v3.1 に対応
:	v3.1にしたら警告がでたので、それの修正
:	機能追加などは無し

2014.03.17
:	cocos2d-x v3.0 rc0に対応

2014.03.10
:   cocos2d-x v3.0 rcに対応
:   伴い、キーワード設定時の文字サイズ指定機能を廃止
:   (すみません、対応できなかった…対応できれば復活させます。。)

2014.03.08 
:   ページ送りのcallbackプロパティを追加

2014.03.07
:   DispSpeedに0設定することで一括表示にするように変更

2014.03.06
:   登録

## LICENSE

The MIT License (MIT)

Copyright (c) 2014 AnzNetJp

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

