# ffmpeg-socket
利用ffmpeg播放socket接收到的视频文件

运行simplest_ffmpeg_men_player.out

服务器： socket/server

思路：接收到数据放到内存，ffmpeg直接从内存中读取数据，解码播放
    
