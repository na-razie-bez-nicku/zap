" Zap filetype plugin: local buffer options
" This file is loaded for buffers with filetype=zap.

" Prefer line comments for operators/plugins.
setlocal commentstring=//\ %s
setlocal comments=s1:/*,mb:*,ex:*/,://

" Keep track of option changes so :setfiletype can cleanly undo them.
let b:undo_ftplugin = 'setlocal commentstring< comments<'
