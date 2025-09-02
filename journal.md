# Development Log

## 09/01/25 - Fiddling around
  * Worked on making a homepage, hooked up `brew install vscode-langservers-extracted` into Helix.

## 08/31/25 - Makefile and clangd, time to work in gstreamer
  * I created a separate gstreamer hello world application which successfully played back a video.
  * I learned a ton about Makefiles and have one I'm happy with in the gstreamer-hello-world project. It was challenging to get the build flags in there because pkg-config required some environment variables, which make has some oddities around.
  * I also got the gstreamer-hello-world makefile to produce the `compile_commands.json`, which is cool!
  * ChatGPT gave me a great starting point for integrating gstreamer and gtk: https://chatgpt.com/share/68b46df3-b1a0-8003-9de2-f7b41169e09c

  * Screwed around a bunch with customizing my prompt line in bash.
  * Got my project running on little mac
  * Got CSS styles working

## 08/29/25 - Initial Development
  * Generated a Makefile using build command from gtk tutorial: https://docs.gtk.org/gtk4/getting_started.html#hello-world-in-c
  * ChatGPT had a great script for auto-generating `compile_commands.json`. This makes Helix's clangd integration happy so it gives us code complete and navigation.
  ```
    echo '[
      {
        "directory": "'$(pwd)'",
        "command": "cc '"$(pkgconf --cflags gtk4)"' gtk-hello-world.c -o gtk-hello-world '"$(pkgconf --libs gtk4)"'",
        "file": "gtk-hello-world.c"
      }
    ]' > compile_commands.json
  ```
 * Ran `brew install gstreamer-runtime` to try and fix video player not working.
 * Apparently, according to chatGPT, I needed to run: `brew install gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-plugins-ugly
`. 
 * After package install, I am getting a "GTK could not find a media module" error.
 * ChatGPT then says to install `libgtk-4-media-gstreamer` package. But I don't see that in brew 🤔.
 * Found this while searching around and it may ccome in handy later: https://stackoverflow.com/questions/32308643/gstreamer-on-mac-plugins-not-working
 * It seems I wasn't integrating gstreamer correctly into the app. I made a separate project called gstreamer-hello-world to learn more about the library in isolation.
