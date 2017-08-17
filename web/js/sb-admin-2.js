$(function() {
    $('#side-menu').metisMenu();
});

//Loads the correct sidebar on window load,
//collapses the sidebar on window resize.
// Sets the min-height of #page-wrapper to window size
$(function() {
    $(window).bind('load resize', function() {
        var topOffset = 50;
        var width = (this.window.innerWidth > 0) ? this.window.innerWidth : this.screen.width;
        if (width < 768) {
            $('div.navbar-collapse').addClass('collapse');
            topOffset = 100; // 2-row-menu
        } else {
            $('div.navbar-collapse').removeClass('collapse');
        }

        var height = ((this.window.innerHeight > 0) ? this.window.innerHeight : this.screen.height) - 1;
        height = height - topOffset;
        if (height < 1) height = 1;
        if (height > topOffset) {
            $('#page-wrapper').css('min-height', (height) + 'px');
        }
    });

    var url = window.location;
    // var element = $('ul.nav a').filter(function() {
    //     return this.href == url;
    // }).addClass('active').parent().parent().addClass('in').parent();
    var element = $('ul.nav a').filter(function() {
        return this.href == url;
    }).addClass('active').parent();

    while (true) {
        if (element.is('li')) {
            element = element.parent().addClass('in').parent();
        } else {
            break;
        }
    }

    function getData() {
        $.get('status.html', {}, function(data, textStatus, jqHXR) {
            $('#status-panel').html(data);
        }, 'html');
    }

    setInterval(getData, 5000);

    function getResponse() {
        $.get('response.html', {}, function(data, textStatus, jqHXR) {
            $('#response-body').html(data);
        }, 'html');
    }

    $('#btn-send').click(function(evt) {
        $.get('../request', {
            command : $('#input-command').val(),
            payload : $('#input-payload').val()
        }, function(data, textStatus, jqHXR) {
            $('#response-body').html(data);
            if (data == 'success') {
                setTimeout(getResponse, 5000);
            }
        }); 
    });
});
