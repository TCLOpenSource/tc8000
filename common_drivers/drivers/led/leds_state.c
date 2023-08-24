// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/device.h>
#include <linux/leds.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/amlogic/scpi_protocol.h>
#include <linux/amlogic/pm.h>
#include <linux/pm_wakeup.h>
#include <linux/pm_wakeirq.h>
#include <linux/amlogic/leds_state.h>
static unsigned int led_mode;
static unsigned int g_blink_times = 1;
static unsigned int g_state;
static unsigned int g_state_brightness;
static unsigned int g_state_audio;
static unsigned int g_state_brightness_audio;

enum user_led_state_t {
	USER_LED_STATE_INVALID = 0,
	USER_LED_STATE_ON, /* full duty */
	USER_LED_STATE_OFF,
	USER_LED_STATE_BREATH,
	USER_LED_STATE_BLINK,
	USER_LED_STATE_ON_DUTY = 5, /* with one parameter: duty to set brightness */
	USER_LED_STATE_BLINK_THREE_ON,
	USER_LED_STATE_BLINK_THREE_OFF,
	USER_LED_STATE_BLINK_ONCE,
	USER_LED_STATE_BLINK_N_ON, /* with one paratmeter, blink times */
	USER_LED_STATE_BLINK_N_ON_DUTY = 10, /* blink times, and duty to set brightness */
	USER_LED_STATE_BLINK_N_OFF,
	USER_LED_STATE_BLINK_N_BREATH,
	USER_LED_STATE_CLEAR_PINMUX,/* deal with kernel shutdown callback */
	USER_LED_STATE_BLINK_OFF,/*the last state is USER_LED_STATE_BLINK_N_OFF*/
	USER_LED_STATE_BLINK_ON = 15,/*the last state is USER_LED_STATE_BLINK_N_ON*/
	USER_LED_STATE_BLINK_BREATH,
	USER_LED_STATE_BLINK_ON_DUTY,
	USER_LED_STATE_BREATH_FAST,
	USER_LED_STATE_BREATH_SLOW,
	USER_LED_STATE_BLINK_FAST = 20,
	USER_LED_STATE_BLINK_FAST_THREE_OFF,
	USER_LED_STATE_BLINK_FAST_THREE_ON,
	USER_LED_STATE_DEFAULT = 23,
};

/*stick parameters store brightness and blink times
 *blink times: high 16 bits
 *state brightness : low 16 bits
 */
static ssize_t blink_times_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return g_blink_times;
}

static ssize_t blink_times_store(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
	int val, res;

	res = sscanf(buf, "%d", &val);
	if (res != 1) {
		dev_err(dev, "Can't parse parameter\n");
		return -EINVAL;
	}

	g_blink_times = val;/*send high 16 bits*/

	return size;
}
static DEVICE_ATTR_RW(blink_times);

static ssize_t state_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return g_state;
}

static ssize_t state_store(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
	int blinkstate, blinktime, brightaftblink, res;

	res = sscanf(buf, "%d,%d,%d", &blinkstate, &blinktime, &brightaftblink);
	if (res != 3) {
		dev_err(dev, "Can't parse parameter\n");
		return -EINVAL;
	}
	g_state = blinkstate;
	g_blink_times = blinktime;
	g_state_brightness = brightaftblink;
	pr_info("%s, g_state = %d, g_blink_times= %d, g_state_brightness = %d\n",
		__func__, g_state, g_blink_times, g_state_brightness);

	res = meson_led_state_set_blink(0, g_state);
	if (res != 0) {
		dev_err(dev, "Can't set led state %s\n", __func__);
		return -EINVAL;
	}

	return size;
}
static DEVICE_ATTR_RW(state);

static ssize_t state_audio_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
		return g_state_audio;
}

static ssize_t state_audio_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t size)
{
	int val, res;

	res = sscanf(buf, "%d", &val);
	if (res != 1) {
		dev_err(dev, "Can't parse parameter\n");
		return -EINVAL;
	}
	g_state_audio = val;

	res = meson_led_state_set_blink(1, g_state_audio);
	if (res != 0) {
		dev_err(dev, "Can't set led state %s\n", __func__);
		return -EINVAL;
	}

	return size;
}

static DEVICE_ATTR_RW(state_audio);

static ssize_t state_brightness_audio_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
		return g_state_brightness_audio;
}

static ssize_t state_brightness_audio_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{
	int val, res;

	res = sscanf(buf, "%d", &val);
	if (res != 1) {
		dev_err(dev, "Can't parse parameter\n");
		return -EINVAL;
	}
	g_state_brightness_audio = val;
	meson_led_state_set_brightness(1, g_state_brightness_audio);
	return size;
}
static DEVICE_ATTR_RW(state_brightness_audio);


static ssize_t blink_off_store(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
	u32 id, times, high_ms, low_ms;
	int res;

	res = sscanf(buf, "%d %d %d %d", &id, &times, &high_ms, &low_ms);
	if (res != 4) {
		pr_err("%s Can't parse! usage:[id times high(ms) low(ms)]\n",
		       DRIVER_NAME);
		return -EINVAL;
	}

	res = meson_led_state_set_blink_off(id, times, high_ms, low_ms, 0, 0);
	if (res) {
		pr_err("%s set blink off fail!\n", DRIVER_NAME);
		return res;
	}

	return size;
}

static DEVICE_ATTR_WO(blink_off);

static ssize_t blink_on_store(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
	u32 id, times, high_ms, low_ms, brightness;
	int res;

	res = sscanf(buf, "%d %d %d %d %d", &id, &times, &high_ms, &low_ms, &brightness);

	if (res != 5) {
		pr_err("%s Can't parse! usage:[id times high(ms) low(ms)]\n",
		       DRIVER_NAME);
		return -EINVAL;
	}

	res = meson_led_state_set_blink_on(id, times, high_ms, low_ms, brightness, 0);
	if (res) {
		pr_err("%s set blink on fail!\n", DRIVER_NAME);
		return res;
	}

	return size;
}

static DEVICE_ATTR_WO(blink_on);

static ssize_t blink_breath_store(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
	u32 id, times, high_ms, low_ms, breath_id;
	int res;

	res = sscanf(buf, "%d %d %d %d %d", &id, &times, &high_ms,
			&low_ms, &breath_id);
	if (res != 5) {
		pr_err("%s Can't parse! usage:[id times high(ms) low(ms)]\n",
		       DRIVER_NAME);
		return -EINVAL;
	}

	res = meson_led_state_set_blink_breath(id, times, high_ms, low_ms, breath_id);
	if (res) {
		pr_err("%s set blink breath fail!\n", DRIVER_NAME);
		return res;
	}

	return size;
}

static DEVICE_ATTR_WO(blink_breath);

static ssize_t breath_store(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
	u32 id, breath;
	int res;

	res = sscanf(buf, "%d %d", &id, &breath);
	if (res != 2) {
		pr_err("%s Can't parse id and breath,usage:[id breath]\n",
		       DRIVER_NAME);
		return -EINVAL;
	}

	res = meson_led_state_set_breath(id, breath);
	if (res) {
		pr_err("%s set breath fail!\n", DRIVER_NAME);
		return res;
	}

	return size;
}

static DEVICE_ATTR_WO(breath);

static ssize_t state_brightness_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	/* TODO: */
	return 0;
}

static ssize_t state_brightness_store(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
	u32 brightness;
	int res;

	res = sscanf(buf, "%d", &brightness);
	if (res != 1) {
		pr_err("%s Can't parse! usage: [brightness]\n",
			DRIVER_NAME);
		return -EINVAL;
	}

	g_state_brightness = brightness;
	res = meson_led_state_set_brightness(0, brightness);
	if (res) {
		pr_err("%s set brightness fail!\n", DRIVER_NAME);
		return res;
	}

	return size;
}

static DEVICE_ATTR_RW(state_brightness);

int meson_led_state_set_brightness(u32 led_id, u32 brightness)
{
	u32 data[3];
	int ret, count;

	if (brightness > LED_STATE_FULL) {
		pr_err("%s %s brightness setting out of range!\n",
			DRIVER_NAME, __func__);
		return -EINVAL;
	}

	data[0] = led_id;
	data[1] = LED_STATE_BRIGHTNESS;
	data[2] = brightness;

	for (count = 0; count < MESON_LEDS_SCPI_CNT; count++) {
		ret = scpi_send_data((void *)data, sizeof(data), SCPI_AOCPU,
			SCPI_CMD_LEDS_STATE, NULL, 0);
		if (ret == 0)
			break;
		mdelay(5);
	}

	if (count == MESON_LEDS_SCPI_CNT) {
		pr_err("%s %s Can't set led brightness count=%d\n",
			DRIVER_NAME, __func__, count);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(meson_led_state_set_brightness);

int meson_led_state_set_breath(u32 led_id, u32 breath_id)
{
	u32 data[3];
	int ret, count;

	if (breath_id > MESON_LEDS_BREATH_MAX_COUNT) {
		pr_err("%s %s Parameter setting out of range!\n",
		       DRIVER_NAME, __func__);
		return -EINVAL;
	}

	data[0] = led_id;
	data[1] = LED_STATE_BREATH;
	data[2] = breath_id;

	for (count = 0; count < MESON_LEDS_SCPI_CNT; count++) {
		ret = scpi_send_data((void *)data, sizeof(data), SCPI_AOCPU,
				     SCPI_CMD_LEDS_STATE, NULL, 0);
		if (ret == 0)
			break;
		mdelay(5);
	}

	if (count == MESON_LEDS_SCPI_CNT) {
		pr_err("%s %s Can't set led breath count=%d\n", DRIVER_NAME,
		       __func__, count);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(meson_led_state_set_breath);

/*to do:Five and six parameters are extended parameters*/
int meson_led_state_set_blink_on(u32 led_id, u32 blink_times,
				     u32 blink_high, u32 blink_low,
				     u32 brightness_high,
				     u32 brightness_low)
{
	u32 data[6];
	int ret, count;

	if (blink_times > MESON_LEDS_MAX_BLINK_CNT ||
	    blink_high > MESON_LEDS_MAX_HIGH_MS ||
	    blink_low > MESON_LEDS_MAX_LOW_MS) {
		pr_err("%s %s Parameter setting out of range!\n",
		       DRIVER_NAME, __func__);
		return -EINVAL;
	}

	/* TODO: brightness_high brightness_low no ready! */
	if (brightness_high || brightness_low)
		pr_info("brightness high and low is no ready!\n");

	data[0] = led_id;
	data[1] = LED_STATE_BLINK_ON;
	data[2] = blink_times;
	data[3] = blink_high;
	data[4] = blink_low;
	data[5] = brightness_high;

	for (count = 0; count < MESON_LEDS_SCPI_CNT; count++) {
		ret = scpi_send_data((void *)data, sizeof(data),
				     SCPI_AOCPU, SCPI_CMD_LEDS_STATE,
				     NULL, 0);
		if (ret == 0)
			break;
		mdelay(5);
	}

	if (count == MESON_LEDS_SCPI_CNT) {
		pr_err("%s %s Can't set led blink on count=%d\n",
		       DRIVER_NAME, __func__, count);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(meson_led_state_set_blink_on);

int meson_led_state_set_blink_breath(u32 led_id, u32 blink_times,
				u32 blink_high, u32 blink_low,
				u32 breath_id)
{
	u32 data[6];
	int ret, count;

	data[0] = led_id;
	data[1] = LED_STATE_BLINK_BREATH;
	data[2] = blink_times;
	data[3] = blink_high;
	data[4] = blink_low;
	data[5] = breath_id;

	for (count = 0; count < MESON_LEDS_SCPI_CNT; count++) {
		ret = scpi_send_data((void *)data, sizeof(data),
				SCPI_AOCPU, SCPI_CMD_LEDS_STATE,
				NULL, 0);
		if (ret == 0)
			break;
		mdelay(5);
	}

	if (count == MESON_LEDS_SCPI_CNT) {
		pr_err("%s %s Can't set led blink on count=%d\n",
			DRIVER_NAME, __func__, count);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(meson_led_state_set_blink_breath);

int meson_led_state_set_blink_off(u32 led_id, u32 blink_times,
				      u32 blink_high, u32 blink_low,
				      u32 brightness_high,
				      u32 brightness_low)
{
	u32 data[5];
	int ret, count;

	/* TODO: brightness_high brightness_low no ready! */
	if (brightness_high || brightness_low)
		pr_info("brightness high and low is no ready!\n");

	data[0] = led_id;
	data[1] = LED_STATE_BLINK_OFF;
	data[2] = blink_times;
	data[3] = blink_high;
	data[4] = blink_low;

	for (count = 0; count < MESON_LEDS_SCPI_CNT; count++) {
		ret = scpi_send_data((void *)data, sizeof(data), SCPI_AOCPU,
				     SCPI_CMD_LEDS_STATE, NULL, 0);
		if (ret == 0)
			break;
		mdelay(5);
	}

	if (count == MESON_LEDS_SCPI_CNT) {
		pr_err("%s %s Can't set led blink off count=%d\n",
		       DRIVER_NAME, __func__, count);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(meson_led_state_set_blink_off);
#define     BLINK_INTERVAL  500
#define     BLINK_FAST_INTERVAL  400
#define     BLINK_SLOW_INTERVAL  600

int meson_led_state_set_blink(u32 led_id, u32 blink_state)
{
	int ret, brightness = 0;

	if (blink_state > USER_LED_STATE_DEFAULT) {
		pr_err("%s %s Parameter setting out of range!\n",
		       DRIVER_NAME, __func__);
		return -EINVAL;
	}
	g_state = blink_state;
	brightness = led_id ? g_state_brightness_audio : g_state_brightness;

	pr_info("g_state = %d, g_blink_times= %d, g_state_brightness = %d\n",
		g_state, g_blink_times, brightness);

	switch (blink_state) {
	case USER_LED_STATE_ON:
		ret = meson_led_state_set_brightness(led_id, 255);
		break;

	case USER_LED_STATE_OFF:
		ret = meson_led_state_set_brightness(led_id, 0);
		break;

	case USER_LED_STATE_BREATH:
		ret = meson_led_state_set_breath(led_id, 1);
		break;

	case USER_LED_STATE_ON_DUTY:
		ret = meson_led_state_set_brightness(led_id, brightness);
		break;

	case USER_LED_STATE_BLINK_THREE_ON:
		ret = meson_led_state_set_blink_on(led_id, 3, BLINK_INTERVAL,
			BLINK_INTERVAL, brightness, 0);
		break;

	case USER_LED_STATE_BLINK_THREE_OFF:
		ret = meson_led_state_set_blink_off(led_id, 3, BLINK_INTERVAL,
			BLINK_INTERVAL, 0, 0);
		break;

	case USER_LED_STATE_BLINK_N_ON_DUTY:
		ret = meson_led_state_set_blink_on(led_id, g_blink_times,
			BLINK_INTERVAL, BLINK_INTERVAL, brightness, 0);
		break;

	case USER_LED_STATE_BLINK_N_BREATH:
		ret = meson_led_state_set_blink_breath(led_id, g_blink_times,
			BLINK_INTERVAL, BLINK_INTERVAL, 1);
		break;

	case USER_LED_STATE_BREATH_FAST:
		ret = meson_led_state_set_breath(led_id, 0);
		break;

	case USER_LED_STATE_BREATH_SLOW:
		ret = meson_led_state_set_breath(led_id, 2);
		break;

	case USER_LED_STATE_BLINK_FAST:
		ret = meson_led_state_set_blink_on(led_id, 3, BLINK_FAST_INTERVAL,
			BLINK_FAST_INTERVAL, 255, 0);
		break;

	case USER_LED_STATE_BLINK_FAST_THREE_ON:
		ret = meson_led_state_set_blink_on(led_id, 3, BLINK_FAST_INTERVAL,
			BLINK_FAST_INTERVAL, 255, 0);
		break;

	case USER_LED_STATE_BLINK_FAST_THREE_OFF:
		ret = meson_led_state_set_blink_off(led_id, 3, BLINK_FAST_INTERVAL,
			BLINK_FAST_INTERVAL, 255, 0);
		break;

	default:
		pr_err("%s %s error blink state\n",
		DRIVER_NAME, __func__);
		return -EINVAL;
	};
	if (ret != 0)
		return -EINVAL;
	return 0;
}
EXPORT_SYMBOL_GPL(meson_led_state_set_blink);

void meson_led_device_create(struct led_classdev *led_cdev)
{
	int rc;

	pr_info("%s, led mode = %d\n", __func__, led_mode);
	rc = device_create_file(led_cdev->dev, &dev_attr_state);
	rc = device_create_file(led_cdev->dev, &dev_attr_blink_times);
	rc = device_create_file(led_cdev->dev, &dev_attr_state_audio);
	rc = device_create_file(led_cdev->dev, &dev_attr_state_brightness_audio);
	rc = device_create_file(led_cdev->dev, &dev_attr_state_brightness);
	rc = device_create_file(led_cdev->dev, &dev_attr_breath);
	rc = device_create_file(led_cdev->dev, &dev_attr_blink_on);
	rc = device_create_file(led_cdev->dev, &dev_attr_blink_off);
	rc = device_create_file(led_cdev->dev, &dev_attr_blink_breath);
	if (rc)
		goto err_out_led_state;

	return;

err_out_led_state:
	device_create_file(led_cdev->dev, &dev_attr_state);
	device_remove_file(led_cdev->dev, &dev_attr_blink_times);
	device_remove_file(led_cdev->dev, &dev_attr_state_audio);
	device_remove_file(led_cdev->dev, &dev_attr_state_brightness_audio);
	device_remove_file(led_cdev->dev, &dev_attr_state_brightness);
	device_remove_file(led_cdev->dev, &dev_attr_breath);
	device_remove_file(led_cdev->dev, &dev_attr_blink_on);
	device_remove_file(led_cdev->dev, &dev_attr_blink_off);
	device_remove_file(led_cdev->dev, &dev_attr_blink_breath);
}

static int meson_led_state_probe(struct platform_device *pdev)
{
	struct led_state_data *data;
	int ret;

	pr_info("%s led state probe start!\n", DRIVER_NAME);
	data = devm_kzalloc(&pdev->dev, sizeof(struct led_state_data),
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->cdev.name = pdev->dev.of_node->name;
	pr_info("pdev->dev.of_node->name=%s\n", pdev->dev.of_node->name);
	ret = led_classdev_register(&pdev->dev, &data->cdev);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to register for %s\n",
			data->cdev.name);
		goto err;
	}

	/*should after led_classdev_register,
	 *the stateled dir should be created first
	 */
	meson_led_device_create(&data->cdev);
	platform_set_drvdata(pdev, data);
	pr_info("%s led state probe over!\n", DRIVER_NAME);

	return 0;

err:
	led_classdev_unregister(&data->cdev);

	return ret;
}

static void meson_led_state_shutdown(struct platform_device *pdev)
{
	int res = 1;

	pr_info("%s, led mode = %d\n", __func__, led_mode);
	switch (led_mode) {
	case 1:
		res = meson_led_state_set_blink(0, USER_LED_STATE_BLINK_THREE_ON);
		break;
	case 2:
		res = meson_led_state_set_blink(0, USER_LED_STATE_BLINK_N_BREATH);
		break;
	case 3:
		res = meson_led_state_set_blink(0, USER_LED_STATE_BLINK_THREE_OFF);
		break;
	default:
		pr_info("unkno ledmode\n");
		break;
	}

	if (res != 0)
		pr_err("Can't set led state\n");

}

#ifdef CONFIG_PM
static int meson_led_state_suspend(struct platform_device *pdev,
				   pm_message_t state)
{
	pr_info("%s, g_state = %d\n", __func__, g_state);
	return 0;
}

static int meson_led_state_resume(struct platform_device *pdev)
{
	pr_info("%s, g_state = %d\n", __func__, g_state);
	//if (is_pm_freeze_mode())
	{
		pr_info("%s, in freeze g_state = %d\n", __func__, g_state);
		if (g_state == 12 || g_state == 3)
			meson_led_state_set_blink(0, USER_LED_STATE_BLINK_THREE_ON);
		else
			meson_led_state_set_blink(0, USER_LED_STATE_BLINK_THREE_OFF);
	}	return 0;
}
#endif

static int meson_led_state_remove(struct platform_device *pdev)
{
	struct led_state_data *data = platform_get_drvdata(pdev);
	struct led_classdev *led_cdev = &data->cdev;

	pr_info("%s, led mode = %d\n", __func__, led_mode);

	device_create_file(led_cdev->dev, &dev_attr_state);
	device_remove_file(led_cdev->dev, &dev_attr_blink_times);
	device_remove_file(led_cdev->dev, &dev_attr_state_audio);
	device_remove_file(led_cdev->dev, &dev_attr_state_brightness_audio);
	device_remove_file(led_cdev->dev, &dev_attr_state_brightness);
	device_remove_file(led_cdev->dev, &dev_attr_breath);
	device_remove_file(led_cdev->dev, &dev_attr_blink_on);
	device_remove_file(led_cdev->dev, &dev_attr_blink_off);
	device_remove_file(led_cdev->dev, &dev_attr_blink_breath);
	led_classdev_unregister(&data->cdev);

	return 0;
}

static const struct of_device_id of_led_state_match[] = {
	{ .compatible = "amlogic,state-led-aocpu", },
	{},
};
MODULE_DEVICE_TABLE(led, of_led_state_match);

static struct platform_driver led_state_driver = {
	.probe		= meson_led_state_probe,
	.remove		= meson_led_state_remove,
	.shutdown   = meson_led_state_shutdown,
#ifdef	CONFIG_PM
	.suspend	= meson_led_state_suspend,
	.resume		= meson_led_state_resume,
#endif
	.driver		= {
		.name	= "led_state",
		.owner	= THIS_MODULE,
		.of_match_table = of_led_state_match,
	},
};

int __init led_state_init(void)
{
	return platform_driver_register(&led_state_driver);
}

void __exit led_state_exit(void)
{
	platform_driver_unregister(&led_state_driver);
}
