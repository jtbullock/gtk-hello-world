# Development Log

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

